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

  #define BUSY  0
  #define READY 1

  typedef enum {CPU,MIC,GPU} platform;

  typedef unsigned int dev_id;
  typedef unsigned int dev_state;


  typedef struct{
    platform    arch;
    dev_id      id;
    dev_state   state;

    // we can add more parameter to create a more
    // complex logic, i.e. bandwidth, peak performance per operation,
    // priority... and others.
  }spike_device;

  typedef struct{
    spike_int      partitions;
    spike_int      *block_rows;
    spike_device   *device;
  }spike_solver_handler;

  spike_solver_handler* analyse_matrix(spike_csr_matrix* M);
  void free_handler ( spike_solver_handler* handler );
  void spike_solver ( spike_csr_matrix* M);

#endif // end of  _SPIKE_CORE_H_ 

