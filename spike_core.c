#include "spike_core.h"

/*
 * Computes the optimal solving approach based on the matrix analysis.
 * Will return the decomposition scheme as well as the number of threads,
 * devices and other intformation.
 *
 * At the moment it only returns the number of partitions to be used in
 * the shared memory implementation.
 */
spike_int analyse_matrix(void)
{
	return 2;
};

void extract_block ( spike_int partitions, spike_int p, spike_csr_matrix* M)
{
	
};

void spike_solver ( spike_csr_matrix* M)
{
	// esto debe ser una logica mas compleja en un futuro
	spike_int P = analyse_matrix();
};
