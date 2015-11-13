#include "spike_matrix.h"

// =========================================================== //
//             CSR matrix definition and functions             //
// =========================================================== //

void free_csr_matrix ( spike_csr_matrix* Matrix )
{
  free ( Matrix->colind ); Matrix->colind = NULL;
  free ( Matrix->rowptr ); Matrix->rowptr = NULL;
  free ( Matrix->coeffs ); Matrix->coeffs = NULL;
  free ( Matrix         ); Matrix    = NULL;
};

spike_csr_matrix* test_csr_matrix (void)
{
  spike_csr_matrix* Matrix = (spike_csr_matrix*) spike_malloc( ALIGN_INT, sizeof(spike_csr_matrix));

	// initialize structure members
  Matrix->nnz    = (spike_int) 13;
  Matrix->ncols  = (spike_int)  5;
  Matrix->nrows  = (spike_int)  5;
  Matrix->colind = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nnz        * sizeof(spike_int  ));
  Matrix->rowptr = (spike_int* ) spike_malloc( ALIGN_INT , (Matrix->nrows +1) * sizeof(spike_int  ));
  Matrix->coeffs = (spike_real*) spike_malloc( ALIGN_REAL, Matrix->nnz        * sizeof(spike_real ));

	// coefficients
	Matrix->coeffs[ 0] = (spike_real)  3.0;	
	Matrix->coeffs[ 1] = (spike_real)  1.0;	
	Matrix->coeffs[ 2] = (spike_real) -1.0;	
	Matrix->coeffs[ 3] = (spike_real)  4.0;	
	Matrix->coeffs[ 4] = (spike_real)  1.0;	
	Matrix->coeffs[ 5] = (spike_real) -1.0;	
	Matrix->coeffs[ 6] = (spike_real)  2.0;	
	Matrix->coeffs[ 7] = (spike_real)  1.0;	
	Matrix->coeffs[ 8] = (spike_real) -1.0;	
	Matrix->coeffs[ 9] = (spike_real)  1.0;	
	Matrix->coeffs[10] = (spike_real)  1.0;	
	Matrix->coeffs[11] = (spike_real) -1.0;	
	Matrix->coeffs[12] = (spike_real)  5.0;	

	// column indices
	Matrix->colind[ 0] = (spike_int) 0;
	Matrix->colind[ 1] = (spike_int) 1;
	Matrix->colind[ 2] = (spike_int) 0;
	Matrix->colind[ 3] = (spike_int) 1;
	Matrix->colind[ 4] = (spike_int) 2;
	Matrix->colind[ 5] = (spike_int) 1;
	Matrix->colind[ 6] = (spike_int) 2;
	Matrix->colind[ 7] = (spike_int) 3;
	Matrix->colind[ 8] = (spike_int) 2;
	Matrix->colind[ 9] = (spike_int) 3;
	Matrix->colind[10] = (spike_int) 4;
	Matrix->colind[11] = (spike_int) 3;
	Matrix->colind[12] = (spike_int) 4;

	// coefficiemts
	Matrix->rowptr[0] = (spike_int)  0;
	Matrix->rowptr[1] = (spike_int)  2;
	Matrix->rowptr[2] = (spike_int)  5;
	Matrix->rowptr[3] = (spike_int)  8;
	Matrix->rowptr[4] = (spike_int) 11;
	Matrix->rowptr[5] = (spike_int) 13;

	return Matrix;
};

void show_csr_matrix( spike_csr_matrix *M)
{
	#ifdef DEBUG
	if( M->ncols < 10 && M->nrows < 10){
	
		spike_debug("\n  ------- CSR matrix -------- \n");
	
		// quick solution, create a buffer to store a dense matrix
		spike_real* D = (spike_real*) spike_malloc( ALIGN_REAL, M->ncols * M->nrows * sizeof(spike_real) );
	
		for(int i=0; i<(M->ncols * M->nrows); i++) D[i] = (spike_real) 0.0;

		// build dense matrix from CSR sparse matrix
		for(int i=0; i<M->nrows; i++)
			for(int j = M->rowptr[i]; j < M->rowptr[i+1]; j++){
				// fprintf(stderr, " Fila %d columna  %d valor %.2f\n", i, M->colind[j], M->coeffs[j]);
				D[i*M->ncols + M->colind[j]] = M->coeffs[j];
			}

		// print matrix
		for(int i=0; i<M->nrows; i++){
			for(int j=0; j<M->ncols; j++){
				spike_real coef = D[i*M->ncols + j];		
				
				if( coef >= 0.0)
					fprintf(stderr, " %.2f", coef);
				else
					fprintf(stderr, "%.2f", coef);
			}
			fprintf(stderr, "\n");
		}

		fprintf(stderr, "\n");

		spike_free( D );
	
	} else {
		fprintf(stderr, "Matrix is too big for showing it! (Dimensions: %d by %d)\n", M->nrows, M->ncols);
	}	

	#endif
}

void show_csr_subset( spike_csr_matrix *M, spike_int first_index, spike_int last_index)
{
	#ifdef DEBUG
    spike_debug("\n  --- CSR sublock --- \n");

    if( last_index >= M->nrows) {
      spike_debug("Last index must be smaller than the number of rows in the matrix!\n");
      last_index = M->nrows;
    }

    spike_int nrows = (last_index - first_index) +1;

    spike_real* D = (spike_real*) spike_malloc( ALIGN_REAL, M->ncols * nrows * sizeof(spike_real) );
    
    for(int i=0; i< (M->ncols * nrows); i++) D[i] = (spike_real) 0.0;

    // build dense matrix from CSR sparse matrix
    for(int i=first_index; i<= last_index; i++)
      for(int j = M->rowptr[i]; j < M->rowptr[i+1]; j++){
        spike_int localIndex = (i - first_index) * M->ncols + M->colind[j];
        D[localIndex] = M->coeffs[j];
      }

    // print matrix
    for(int i=0; i<nrows; i++){
      for(int j=0; j<M->ncols; j++){
        spike_real coef = D[i*M->ncols + j];		
        
        if( coef >= 0.0)
          fprintf(stderr, " %.2f", coef);
        else
          fprintf(stderr, "%.2f", coef);
      }
      fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
    spike_free( D );
	#endif
}


spike_csr_matrix* load_csr_matrix    ( const char* filename)
{
  spike_csr_matrix* Matrix = (spike_csr_matrix*) spike_malloc( ALIGN_INT, sizeof(spike_csr_matrix));

  Matrix->nnz    = 4;
  Matrix->ncols  = 8;
  Matrix->nrows  = 8;
  Matrix->rowptr = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nnz       * sizeof(spike_int  ));
  Matrix->coeffs = (spike_real*) spike_malloc( ALIGN_REAL, Matrix->nnz       * sizeof(spike_real ));
  Matrix->colind = (spike_int* ) spike_malloc( ALIGN_INT , (Matrix->nrows+1) * sizeof(spike_int  ));

  // TODO read data from file

  return Matrix;
};



// =========================================================== //
//             CSC matrix definition and functions             //
// =========================================================== //

void free_csc_matrix ( spike_csc_matrix* Matrix )
{
  free ( Matrix->colptr ); Matrix->rowind = NULL;
  free ( Matrix->rowind ); Matrix->colptr = NULL;
  free ( Matrix->coeffs ); Matrix->coeffs = NULL;
  free ( Matrix         ); Matrix         = NULL;
};

spike_csc_matrix* test_csc_matrix (void)
{
  spike_csc_matrix* Matrix = (spike_csc_matrix*) spike_malloc( ALIGN_INT, sizeof(spike_csc_matrix));

	// initialize structure members
  Matrix->nnz    = (spike_int) 13;
  Matrix->ncols  = (spike_int)  5;
  Matrix->nrows  = (spike_int)  5;
  Matrix->rowind = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nnz        * sizeof(spike_int  ));
  Matrix->colptr = (spike_int* ) spike_malloc( ALIGN_INT , (Matrix->nrows +1) * sizeof(spike_int  ));
  Matrix->coeffs = (spike_real*) spike_malloc( ALIGN_REAL, Matrix->nnz        * sizeof(spike_real ));

	// coefficients
	Matrix->coeffs[ 0] = (spike_real)  3.0;	
	Matrix->coeffs[ 1] = (spike_real)  1.0;	
	Matrix->coeffs[ 2] = (spike_real) -1.0;	
	Matrix->coeffs[ 3] = (spike_real)  4.0;	
	Matrix->coeffs[ 4] = (spike_real)  1.0;	
	Matrix->coeffs[ 5] = (spike_real) -1.0;	
	Matrix->coeffs[ 6] = (spike_real)  2.0;	
	Matrix->coeffs[ 7] = (spike_real)  1.0;	
	Matrix->coeffs[ 8] = (spike_real) -1.0;	
	Matrix->coeffs[ 9] = (spike_real)  1.0;	
	Matrix->coeffs[10] = (spike_real)  1.0;	
	Matrix->coeffs[11] = (spike_real) -1.0;	
	Matrix->coeffs[12] = (spike_real)  5.0;	

	// column indices
	Matrix->rowind[ 0] = (spike_int) 0;
	Matrix->rowind[ 1] = (spike_int) 1;
	Matrix->rowind[ 2] = (spike_int) 0;
	Matrix->rowind[ 3] = (spike_int) 1;
	Matrix->rowind[ 4] = (spike_int) 2;
	Matrix->rowind[ 5] = (spike_int) 1;
	Matrix->rowind[ 6] = (spike_int) 2;
	Matrix->rowind[ 7] = (spike_int) 3;
	Matrix->rowind[ 8] = (spike_int) 2;
	Matrix->rowind[ 9] = (spike_int) 3;
	Matrix->rowind[10] = (spike_int) 4;
	Matrix->rowind[11] = (spike_int) 3;
	Matrix->rowind[12] = (spike_int) 4;

	// coefficiemts
	Matrix->colptr[0] = (spike_int)  0;
	Matrix->colptr[1] = (spike_int)  2;
	Matrix->colptr[2] = (spike_int)  5;
	Matrix->colptr[3] = (spike_int)  8;
	Matrix->colptr[4] = (spike_int) 11;
	Matrix->colptr[5] = (spike_int) 13;

	return Matrix;
};

void show_csc_matrix( spike_csc_matrix *M)
{
  // TODO implement
};

void show_csc_subset( spike_csc_matrix *M, spike_int first_index, spike_int last_index)
{

};


spike_csc_matrix* load_csc_matrix    ( const char* filename)
{
  spike_csc_matrix* Matrix = (spike_csc_matrix*) spike_malloc( ALIGN_INT, sizeof(spike_csc_matrix));

  Matrix->nnz    = 4;
  Matrix->ncols  = 8;
  Matrix->nrows  = 8;
  Matrix->rowind = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nnz       * sizeof(spike_int  ));
  Matrix->colptr = (spike_int* ) spike_malloc( ALIGN_INT , (Matrix->nrows+1) * sizeof(spike_int  ));
  Matrix->coeffs = (spike_real*) spike_malloc( ALIGN_REAL, Matrix->nnz       * sizeof(spike_real ));

  // TODO read data from file

  return Matrix;
};


// =========================================================== //
//             Format conversion routines                      //
// =========================================================== //

spike_csr_matrix* csc2csr( spike_csc_matrix* CSC )
{
	spike_csr_matrix* CSR = (spike_csr_matrix*) \
													spike_malloc( ALIGN_INT, sizeof(spike_csr_matrix));
	
	CSR->ncols  = CSC->ncols;
	CSR->nrows  = CSC->nrows;
	CSR->nnz    = CSC->nnz;
	CSR->rowptr = (spike_int* ) spike_malloc( ALIGN_INT , (CSR->nrows+1) * sizeof(spike_int ));
  CSR->colind = (spike_int* ) spike_malloc( ALIGN_INT ,  CSR->nnz	     * sizeof(spike_int ));
	CSR->coeffs = (spike_real*) spike_malloc( ALIGN_REAL,  CSR->nnz      * sizeof(spike_real));


	spike_int job[8] = {1,0,0,0,0,0,0,0};
	// TESTIT	job[5] = 1;
	spike_int info;

	CSRCSC( job, &CSR->nrows, CSR->coeffs, CSR->colind, CSR->rowptr, CSC->coeffs, CSC->rowind, CSC->colptr, &info);

	free_csc_matrix( CSC );

	spike_debug("Matrix was converted from CSC to CSR succesfully!\n");

	return CSR;
};

spike_csc_matrix* csr2csc( spike_csr_matrix* CSR )
{
	spike_csc_matrix* CSC = (spike_csc_matrix*) \
													spike_malloc( ALIGN_INT, sizeof(spike_csc_matrix));
	
	CSC->ncols  = CSR->ncols;
	CSC->nrows  = CSR->nrows;
	CSC->nnz    = CSR->nnz;
	CSC->colptr = (spike_int* ) spike_malloc( ALIGN_INT , (CSC->ncols+1) * sizeof(spike_int ));
  CSC->rowind = (spike_int* ) spike_malloc( ALIGN_INT ,  CSC->nnz	     * sizeof(spike_int ));
	CSC->coeffs = (spike_real*) spike_malloc( ALIGN_REAL,  CSC->nnz      * sizeof(spike_real));

	spike_int job[8] = {0,0,0,0,0,0,0,0};
	// TESTIT	job[5] = 1;
	spike_int info;

	CSRCSC( job, &CSR->nrows, CSR->coeffs, CSR->colind, CSR->rowptr, CSC->coeffs, CSC->rowind, CSC->colptr, &info);

	free_csr_matrix( CSR );

	spike_debug("Matrix was converted from CSR to CSC succesfully!\n");

	return CSC;
}
