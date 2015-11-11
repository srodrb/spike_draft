#include "spike_matrix.h"

void free_csr_matrix ( spike_csr_matrix* Matrix )
{
  free ( Matrix->i ); Matrix->i = NULL;
  free ( Matrix->j ); Matrix->j = NULL;
  free ( Matrix->c ); Matrix->c = NULL;
  free ( Matrix    ); Matrix    = NULL;
};

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

