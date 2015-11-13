/*
 * =====================================================================================
 *
 *       Filename:  spike_matrix.h
 *
 *    Description:  Matrix structure definition for the SPIKE solver.
 *                  It uses CSR format by default.
 *
 *        Version:  1.0
 *        Created:  10/11/2015 18:50:10
 *       Revision:  none
 *       Compiler:  nvcc - icc
 *
 *         Author:  Samuel Rodriguez Bernabeu
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#ifndef _SPIKE_MATRIX_H_
  #define _SPIKE_MATRIX_H_ 
  
  #include "spike_common.h"
  
  // =========================================================== //
  //             CSR matrix definition and functions             //
  // =========================================================== //

  typedef struct
  {
    spike_int    ncols;
    spike_int    nrows;
    spike_int    nnz;
    spike_int   *rowptr;
    spike_int   *colind;
    spike_real  *coeffs;
  
  }spike_csr_matrix;

  void free_csr_matrix ( spike_csr_matrix* Matrix );

	void show_csr_matrix ( spike_csr_matrix* M );

	void show_csr_subset( spike_csr_matrix *M, spike_int first_index, spike_int last_index);
	
	spike_csr_matrix* test_csr_matrix (void); 
 
  spike_csr_matrix* load_csr_matrix ( const char* filename);

  spike_csr_matrix* create_csr_matrix (   spike_int ncols,
                                          spike_int nrows,
                                          spike_int nnz,
                                          spike_int *colind, 
                                          spike_int *rowptr, 
                                          spike_real *coefs); 


  // =========================================================== //
  //             CSC matrix definition and functions             //
  // =========================================================== //

  typedef struct
  {
    spike_int    ncols;
    spike_int    nrows;
    spike_int    nnz;
    spike_int   *colptr;
    spike_int   *rowind;
    spike_real  *coeffs;
  
  }spike_csc_matrix;

  void free_csc_matrix ( spike_csc_matrix* Matrix );

	void show_csc_matrix ( spike_csc_matrix* M );

	void show_csc_subset( spike_csc_matrix *M, spike_int first_index, spike_int last_index);
	
	spike_csc_matrix* test_csc_matrix (void); 
 
  spike_csc_matrix* load_csc_matrix ( const char* filename);

  spike_csc_matrix* create_csc_matrix (   spike_int  ncols,
                                          spike_int  nrows,
                                          spike_int  nnz,
                                          spike_int  *colptr, 
                                          spike_int  *rowind, 
                                          spike_real *coefs); 




#endif // end of _SPIKE_MATRIX_H_ 

