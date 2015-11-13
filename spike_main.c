/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Shared Memory Heterogeneous solver for large systems of linear
 *                 equations, based on the ideas of SPIKE partition approach.
 *                 It is able to handle multiple GPU devices.
 *
 *        Version:  1.0
 *        Created:  10/11/2015 09:14:01
 *       Revision:  none
 *       Compiler:  nvcc - icc
 *
 *         Author:  Samuel Rodriguez Bernabeu
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#include "spike_common.h"
#include "spike_matrix.h"
#include "spike_core.h"
#include "spike_algebra.h"

int main(int argc, const char *argv[])
{
	double starttime, solvertime;

	// ========================================= //
	//            Solver Execution               //
	// ========================================= //
	
	spike_wellcome_header();

  spike_csr_matrix* M = test_csr_matrix();

	show_csr_matrix( M );

	// ahora tengo que crear un algoritmo de particionado eficiente.
	// creo que la forma más simple de hacerlo es por medio de un histograma.

	// necesito un metodo para computar el bandwidth maximo de la matriz. !!	

  spike_opentimer( &starttime );
  
  spike_factorize( M );

  spike_closetimer( &starttime, &solvertime);


  free_csr_matrix( M);

	superlu_solve();

  spike_debug("Factorization took %.12f seconds\n", solvertime);
  return 0;
}


