/*
 * =====================================================================================
 *
 *       Filename:  spike_algebra.h
 *
 *    Description:  Algebra subroutines for solving central blocks and reduced
 *    							linear systems.
 *
 *    							Interfaces with SuperLU and Intel Pardiso.
 *
 *        Version:  1.0
 *        Created:  13/11/15 13:02:55
 *       Revision:  none
 *       Compiler:  nvcc - icc
 *
 *         Author:  Samuel Rodriguez Bernabeu
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#ifndef _SPIKE_ALGEBRA_H_
	#define _SPIKE_ALGEBRA_H_
	
	#include "spike_common.h"
	
	#ifdef DOUBLE_PREC
		#include "slu_mt_sdefs.h"
	#else
		#include "slu_mt_ddefs.h"
	#endif

	void superlu_solve( void );

#endif // _SPIKE_ALGEBRA_H_ 

