#include "spike_common.h"

void* spike_malloc( const int alignment, const size_t size)
{
  void* buffer = malloc( size );
    
  if ( buffer ) {
    return buffer;
  } else {
    fprintf(stderr,"Cant allocate memory!\n");
    abort();
  }
};

void spike_free (void* ptr) 
{
	free(ptr);
}

void spike_wellcome_header ( void )
{
	fprintf(stderr, "\n\
                     __    __            _______..______    __   __  ___  _______  \n \
                   |  |  |  |          /       ||   _  \\  |  | |  |/  / |   ____| \n \
                   |  |__|  |  ______ |   (----`|  |_)  | |  | |  '  /  |  |__    \n \
                   |   __   | |______| \\   \\    |   ___/  |  | |    <   |   __|   \n \
                   |  |  |  |      .----)   |   |  |      |  | |  .  \\  |  |____  \n \
                   |__|  |__|      |_______/    | _|      |__| |__|\\__\\ |_______| \n \
									\n\n");
};
