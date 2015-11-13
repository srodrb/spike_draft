#include "spike_algebra.h"

/*
 * Decomposes the matrix into L and U factors and then
 * uses them to solve various linear systems.
 */

void superlu_solve( void )
{
    SuperMatrix							 A, A1, L, U;
    SuperMatrix							 B, B1, X;
    NCformat								*Astore;
    SCPformat								*Lstore;
    NCPformat								*Ustore;
    int_t										 nprocs;
    fact_t									 fact;
    trans_t									 trans;
    yes_no_t								 refact, usepr;
    equed_t									 equed;
    float										*a, *a1;
    int_t										*asub, *xa, *asub1, *xa1;
    int_t										*perm_c; /* column permutation vector */
    int_t										*perm_r; /* row permutations from partial pivoting */
    void										*work;
    superlumt_options_t      superlumt_options;
    int_t                    info, lwork, nrhs, ldx, panel_size, relax;
    int_t                    m, n, nnz, permc_spec, i;
    float                   *rhsb, *rhsx, *xact;
    float                   *R, *C;
    float                   *ferr, *berr;
    float                    u, drop_tol, rpg, rcond;
    superlu_memusage_t       superlu_memusage;

    /* Default parameters to control factorization. */
    nprocs = 1;
    fact  = EQUILIBRATE;
    trans = NOTRANS;
    equed = NOEQUIL;
    refact= NO;
    panel_size = sp_ienv(1);
    relax = sp_ienv(2);
    u     = 1.0;
    usepr = NO;
    drop_tol = 0.0;
    lwork = 0;
    nrhs  = 1;

    if ( lwork > 0 ) {
			work = SUPERLU_MALLOC(lwork);
			spike_debug("Use work space of size LWORK = " IFMT " bytes\n", lwork);
	
			if ( !work ) 
	    	SUPERLU_ABORT("SLINSOLX: cannot allocate work[]");
    }

    if ( !(a1    = floatMalloc(nnz)) ) SUPERLU_ABORT("Malloc fails for a1[]."   );
    if ( !(asub1 = intMalloc  (nnz)) ) SUPERLU_ABORT("Malloc fails for asub1[].");
    if ( !(xa1   = intMalloc  (n+1)) ) SUPERLU_ABORT("Malloc fails for xa1[]."  );
    
		for (i = 0; i < nnz; ++i) {
        a1[i] = a[i];
				asub1[i] = asub[i];
    }

    for (i = 0; i < n+1; ++i) xa1[i] = xa[i];

    sCreate_CompCol_Matrix(&A, m, n, nnz, a, asub, xa, SLU_NC, SLU_S, SLU_GE);
    Astore = A.Store;
    spike_debug("Dimension " IFMT "x" IFMT "; # nonzeros " IFMT "\n", A.nrow, A.ncol, Astore->nnz);
    
    if (!(rhsb = floatMalloc(m * nrhs))) SUPERLU_ABORT("Malloc fails for rhsb[].");
    if (!(rhsx = floatMalloc(m * nrhs))) SUPERLU_ABORT("Malloc fails for rhsx[].");
    sCreate_Dense_Matrix(&B, m, nrhs, rhsb, m, SLU_DN, SLU_S, SLU_GE);
    sCreate_Dense_Matrix(&X, m, nrhs, rhsx, m, SLU_DN, SLU_S, SLU_GE);
    xact = floatMalloc(n * nrhs);
    ldx = n;
    sGenXtrue(n, nrhs, xact, ldx);
    sFillRHS(trans, nrhs, xact, ldx, &A, &B);
    
    if (!(perm_r = intMalloc(m))) SUPERLU_ABORT("Malloc fails for perm_r[].");
    if (!(perm_c = intMalloc(n))) SUPERLU_ABORT("Malloc fails for perm_c[].");
    if (!(R = (float *) SUPERLU_MALLOC(A.nrow * sizeof(float)))) 
        SUPERLU_ABORT("SUPERLU_MALLOC fails for R[].");
    if ( !(C = (float *) SUPERLU_MALLOC(A.ncol * sizeof(float))) )
        SUPERLU_ABORT("SUPERLU_MALLOC fails for C[].");
    if ( !(ferr = (float *) SUPERLU_MALLOC(nrhs * sizeof(float))) )
        SUPERLU_ABORT("SUPERLU_MALLOC fails for ferr[].");
    if ( !(berr = (float *) SUPERLU_MALLOC(nrhs * sizeof(float))) ) 
        SUPERLU_ABORT("SUPERLU_MALLOC fails for berr[].");

    /*
     * Get column permutation vector perm_c[], according to permc_spec:
     *   permc_spec = 0: natural ordering 
     *   permc_spec = 1: minimum degree ordering on structure of A'*A
     *   permc_spec = 2: minimum degree ordering on structure of A'+A
     *   permc_spec = 3: approximate minimum degree for unsymmetric matrices
     */    	
    permc_spec = 1;
    get_perm_c(permc_spec, &A, perm_c);

    superlumt_options.nprocs = nprocs;
    superlumt_options.fact = fact;
    superlumt_options.trans = trans;
    superlumt_options.refact = refact;
    superlumt_options.panel_size = panel_size;
    superlumt_options.relax = relax;
    superlumt_options.usepr = usepr;
    superlumt_options.drop_tol = drop_tol;
    superlumt_options.diag_pivot_thresh = u;
    superlumt_options.SymmetricMode = NO;
    superlumt_options.PrintStat = NO;
    superlumt_options.perm_c = perm_c;
    superlumt_options.perm_r = perm_r;
    superlumt_options.work = work;
    superlumt_options.lwork = lwork;
    
		if ( !(superlumt_options.etree = intMalloc(n)) )
			SUPERLU_ABORT("Malloc fails for etree[].");
    if ( !(superlumt_options.colcnt_h = intMalloc(n)) )
			SUPERLU_ABORT("Malloc fails for colcnt_h[].");
    if ( !(superlumt_options.part_super_h = intMalloc(n)) )
			SUPERLU_ABORT("Malloc fails for colcnt_h[].");
    
    /* ------------------------------------------------------------
       WE SOLVE THE LINEAR SYSTEM FOR THE FIRST TIME: AX = B
       ------------------------------------------------------------*/
    psgssvx(nprocs, &superlumt_options, &A, perm_c, perm_r,
	    &equed, R, C, &L, &U, &B, &X, &rpg, &rcond,
	    ferr, berr, &superlu_memusage, &info);

    if ( info == 0 || info == n+1 ) {
			spike_debug("Recip. pivot growth = %e\n", rpg);
			spike_debug("Recip. condition number = %e\n", rcond);
			spike_debug("%8s%16s%16s\n", "rhs", "FERR", "BERR");
			
			for (i = 0; i < nrhs; ++i)
				spike_debug(IFMT "%16e%16e\n", i+1, ferr[i], berr[i]);
					 
			Lstore = (SCPformat *) L.Store;
			Ustore = (NCPformat *) U.Store;
			
			spike_debug("No of nonzeros in factor L = " IFMT "\n", Lstore->nnz);
			spike_debug("No of nonzeros in factor U = " IFMT "\n", Ustore->nnz);
			spike_debug("No of nonzeros in L+U = " IFMT "\n", Lstore->nnz + Ustore->nnz - n);
			spike_debug("L\\U MB %.3f\ttotal MB needed %.3f\texpansions " IFMT "\n",
			
			superlu_memusage.for_lu/1e6, superlu_memusage.total_needed/1e6,
			superlu_memusage.expansions);
				 
			fflush(stdout);
    } else if ( info > 0 && lwork == -1 ) { 
			spike_debug("** Estimated memory: " IFMT " bytes\n", info - n);
    }

    spike_debug("First system: psgssvx(): info " IFMT "\n----\n", info);

    Destroy_CompCol_Matrix(&A);
    Destroy_SuperMatrix_Store(&B);

    /* ------------------------------------------------------------
       NOW WE SOLVE ANOTHER LINEAR SYSTEM: A1*X = B1
       ONLY THE SPARSITY PATTERN OF A1 IS THE SAME AS THAT OF A.
       ------------------------------------------------------------*/
    superlumt_options.refact = YES;
    sCreate_CompCol_Matrix(&A1, m, n, nnz, a1, asub1, xa1, SLU_NC, SLU_S, SLU_GE);
    sCreate_Dense_Matrix(&B1, m, nrhs, rhsb, m, SLU_DN, SLU_S, SLU_GE);

    psgssvx(nprocs, &superlumt_options, &A1, perm_c, perm_r,
	    &equed, R, C, &L, &U, &B1, &X, &rpg, &rcond,
	    ferr, berr, &superlu_memusage, &info);

    if ( info == 0 || info == n+1 ) {

	printf("Recip. pivot growth = %e\n", rpg);
	printf("Recip. condition number = %e\n", rcond);
	printf("%8s%16s%16s\n", "rhs", "FERR", "BERR");
	for (i = 0; i < nrhs; ++i) {
	    printf(IFMT "%16e%16e\n", i+1, ferr[i], berr[i]);
	}
	       
        Lstore = (SCPformat *) L.Store;
        Ustore = (NCPformat *) U.Store;
	printf("No of nonzeros in factor L = " IFMT "\n", Lstore->nnz);
    	printf("No of nonzeros in factor U = " IFMT "\n", Ustore->nnz);
    	printf("No of nonzeros in L+U = " IFMT "\n", Lstore->nnz + Ustore->nnz - n);
	printf("L\\U MB %.3f\ttotal MB needed %.3f\texpansions " IFMT "\n",
	       superlu_memusage.for_lu/1e6, superlu_memusage.total_needed/1e6,
	       superlu_memusage.expansions);
	     
	fflush(stdout);

    } else if ( info > 0 && lwork == -1 ) {
        printf("** Estimated memory: " IFMT " bytes\n", info - n);
    }

    printf("Second system: psgssvx(): info " IFMT "\n", info);

    SUPERLU_FREE (rhsb);
    SUPERLU_FREE (rhsx);
    SUPERLU_FREE (xact);
    SUPERLU_FREE (perm_r);
    SUPERLU_FREE (perm_c);
    SUPERLU_FREE (R);
    SUPERLU_FREE (C);
    SUPERLU_FREE (ferr);
    SUPERLU_FREE (berr);
    Destroy_CompCol_Matrix(&A1);
    Destroy_SuperMatrix_Store(&B1);
    Destroy_SuperMatrix_Store(&X);
    SUPERLU_FREE (superlumt_options.etree);
    SUPERLU_FREE (superlumt_options.colcnt_h);
    SUPERLU_FREE (superlumt_options.part_super_h);
    if ( lwork == 0 ) {
        Destroy_SuperNode_SCP(&L);
        Destroy_CompCol_NCP(&U);
    } else if ( lwork > 0 ) {
        SUPERLU_FREE(work);
    }
};

