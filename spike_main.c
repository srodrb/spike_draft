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

int main(int argc, const char *argv[])
{
  spike_debug("Vamos con el programa!\n");

  spike_csr_matrix* M = load_csr_matrix("inputfile.bin");

  free_csr_matrix( M);

  spike_debug("Fin del programa\n");
  return 0;
}


