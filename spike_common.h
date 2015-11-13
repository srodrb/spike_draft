/*
 * =====================================================================================
 *
 *       Filename:  spike_common.h
 *
 *    Description:  Common functions for the SPIKE solver
 *
 *        Version:  1.0
 *        Created:  10/11/2015 09:16:58
 *       Revision:  none
 *       Compiler:  nvcc - icc
 *
 *         Author:  Samuel Rodriguez Bernabeu
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#ifndef _SPIKE_COMMON_H_
  #define _SPIKE_COMMON_H_

  #define ALIGN_INT  32
  #define ALIGN_REAL 64

	#include <stdio.h>
	#include <stdlib.h>
  #include <sys/time.h>

  #include "spike_debug.h"

  typedef int spike_int;

  #ifndef DOUBLE_PREC
    typedef float spike_real;
		#define CSRCSC mkl_scsrcsc 
  #else
    typedef double spike_real;
		#define CSRCSC mkl_dcsrcsc 
  #endif
  
  void* spike_malloc ( const int aligment, const size_t size); 	
	void  spike_free   ( void* ptr );

	void spike_wellcome_header ( void );

  void spike_opentimer  ( double *starttime);
  void spike_closetimer ( double *starttime, double *apptime );

#endif // end of _SPIKE_COMMON_H_ 
