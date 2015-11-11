#include "spike_matrix.h"

void free_csr_matrix ( spike_csr_matrix* Matrix )
{
  free ( Matrix->i ); Matrix->i = NULL;
  free ( Matrix->j ); Matrix->j = NULL;
  free ( Matrix->c ); Matrix->c = NULL;
  free ( Matrix    ); Matrix    = NULL;
};

spike_csr_matrix* test_csr_matrix (void)
{
  spike_csr_matrix* Matrix = (spike_csr_matrix*) spike_malloc( ALIGN_INT, sizeof(spike_csr_matrix));

  Matrix->nnz   = 13;
  Matrix->ncols = 5;
  Matrix->nrows = 5;

  Matrix->i = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nnz      * sizeof(spike_int  ));
  Matrix->j = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nrows +1 * sizeof(spike_int  ));
  Matrix->c = (spike_real*) spike_malloc( ALIGN_REAL, Matrix->nnz      * sizeof(spike_real ));

	Matrix->c[ 0] =  3;	Matrix->i[ 0] = 0;
	Matrix->c[ 1] =  1;	Matrix->i[ 1] = 1;
	Matrix->c[ 2] = -1;	Matrix->i[ 2] = 0;
	Matrix->c[ 3] =  4;	Matrix->i[ 3] = 1;
	Matrix->c[ 4] =  1;	Matrix->i[ 4] = 2;
	Matrix->c[ 5] = -1;	Matrix->i[ 5] = 1;
	Matrix->c[ 6] =  2;	Matrix->i[ 6] = 2;
	Matrix->c[ 7] =  1;	Matrix->i[ 7] = 3;
	Matrix->c[ 8] = -1;	Matrix->i[ 8] = 2;
	Matrix->c[ 9] =  1;	Matrix->i[ 9] = 3;
	Matrix->c[10] =  1;	Matrix->i[10] = 4;
	Matrix->c[11] = -1;	Matrix->i[11] = 3;
	Matrix->c[12] =  5;	Matrix->i[12] = 4;

	Matrix->j[0] = 0;
	Matrix->j[1] = 2;
	Matrix->j[2] = 5;
	Matrix->j[3] = 8;
	Matrix->j[4] = 11;
	Matrix->j[5] = 13;

	return Matrix;
};

void show_csr_matrix( spike_csr_matrix *M)
{
	#ifdef DEBUG
	if( M->ncols < 10 && M->nrows < 10){
	
		fprintf(stderr, "\n  --- CSR matrix --- \n");
	
		// quick solution, create a buffer to store a dense matrix
		spike_real* D = (spike_real*) spike_malloc( ALIGN_REAL, M->ncols * M->nrows * sizeof(spike_real) );
	
		for(int i=0; i<(M->ncols * M->nrows); i++) D[i] = (spike_real) 0.0;

		// build dense matrix from CSR sparse matrix
		for(int i=0; i<M->nrows; i++){ 
			for(int j = M->j[i]; j < M->j[i+1]; j++){
				// fprintf(stderr, " Fila %d columna  %d valor %.2f\n", i, M->i[j], M->c[j]);
				D[i*M->ncols + M->i[j]] = M->c[j];
			}
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



spike_csr_matrix* load_csr_matrix    ( const char* filename)
{
  spike_csr_matrix* Matrix = (spike_csr_matrix*) spike_malloc( ALIGN_INT, sizeof(spike_csr_matrix));

  Matrix->nnz   = 4;
  Matrix->ncols = 8;
  Matrix->nrows = 8;

  Matrix->i = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nnz   * sizeof(spike_int  ));
  Matrix->c = (spike_real*) spike_malloc( ALIGN_REAL, Matrix->nnz   * sizeof(spike_real ));
  Matrix->j = (spike_int* ) spike_malloc( ALIGN_INT , Matrix->nrows * sizeof(spike_int  ));

  // TODO read data from file

  return Matrix;
};

