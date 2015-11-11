/*
 * =====================================================================================
 *
 *       Filename:  spike_core.h
 *
 *    Description:  Core functions for matrix computations.
 *    							Includes from factorizations (LU,ILU(n)) to direct solver interfaces.
 *
 *        Version:  1.0
 *        Created:  11/11/15 15:00:26
 *       Revision:  none
 *       Compiler:  nvcc - icc
 *
 *         Author:  Samuel Rodriguez Bernabeu 
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#ifndef _SPIKE_CORE_H_
	#define _SPIKE_CORE_H_

	#include "spike_common.h"
	#include "spike_matrix.h"

	void extract_block ( spike_int partitions, spike_int p);

#endif // end of  _SPIKE_CORE_H_ 

