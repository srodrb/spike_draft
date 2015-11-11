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
