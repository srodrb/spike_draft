#include "spike_core.h"

/*
 * Computes the optimal solving approach based on the matrix analysis.
 * Will return the decomposition scheme as well as the number of threads,
 * devices and other intformation.
 *
 * The partition has to be refined to be smarter. At the moment it does not
 * include any complex logic nor load balancing.
 *
 * At the moment it only returns the number of partitions to be used in
 * the shared memory implementation.
 */
spike_solver_handler* analyse_matrix(spike_csr_matrix* M)
{
  spike_solver_handler* handler = (spike_solver_handler*) spike_malloc( ALIGN_INT, 
                                 sizeof(spike_solver_handler));

  handler->partitions = 2;
  handler->block_rows = (spike_int*) spike_malloc( ALIGN_INT,  
                        handler->partitions * sizeof(spike_int));

  handler->device = (spike_device*) spike_malloc( ALIGN_INT,   
                    handler->partitions * sizeof( spike_device));

  spike_int regular_blockdim = (spike_int) (M->nrows / handler->partitions );
  spike_int extra_rows       = (spike_int) (M->nrows % handler->partitions );


  if ( extra_rows == 0 ){
    for(int i=0; i < handler->partitions; i++)
      handler->block_rows[i] = regular_blockdim; 
    
  } else {

    for(int i=0; i<(handler->partitions -1); i++)
      handler->block_rows[i] = regular_blockdim; 
   
    handler->block_rows[handler->partitions -1] = regular_blockdim + extra_rows;
  }
  
  for( int i=0; i < handler->partitions; i++)
    handler->device[i] = (spike_device) {CPU,0,READY};


  #ifdef DEBUG
    spike_debug("Spike handler: solving approach:\n");
    spike_debug("--------------------------------\n");
    
    for(int i=0; i < handler->partitions; i++){
      spike_debug("    Partition %d \n", i);
      spike_debug("    Device (arch,id,status)  (%d,%d,%d)\n", 
                  handler->device[i].arch, handler->device[i].id, handler->device[i].state);
      spike_debug("    Number of rows in block: %d\n", handler->block_rows[i]);
      spike_debug("--------------------------------\n");
    }
  #endif


  return handler;
};

void free_handler ( spike_solver_handler* handler )
{
  free( handler->block_rows ); handler->block_rows = NULL;
  free( handler->device     ); handler->device     = NULL;
  free( handler             ); handler             = NULL;
};

void spike_factorize ( spike_csr_matrix* M)
{
	// esto debe ser una logica mas compleja en un futuro
	spike_solver_handler* handler = analyse_matrix( M );


	// ahora tenemos que descomponer la matriz e invertir los bloques.
	show_csr_subset( M, 0, 2);
	show_csr_subset( M, 1, 3);

	free_handler( handler );
};
