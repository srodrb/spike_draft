#include "spike_common.h"

void* spike_malloc( const int alignment, const size_t size)
{
  void* buffer = malloc( size );
  spike_check( buffer, "Cant allocate memory correctly");

  return buffer;

error:
  free  ( buffer );
  abort ();
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


void spike_opentimer  ( double *starttime )
{
  *starttime = 0.0;
  struct timeval mytime;
  gettimeofday(&mytime, (struct timezone*) 0);
  *starttime = (double) (mytime.tv_sec + mytime.tv_usec*1.0e-6);
};

void spike_closetimer ( double *starttime, double *apptime )
{
  double endtime = 0.0;
  spike_opentimer( &endtime );
  *apptime = endtime - *starttime;
}
