#include <stdio.h>
#include <sys/time.h>
#include "memr.h"

#define LENGTH 120000000
#define CACHE_LINE 64
#define LOOP 6000000

int main( void )
{
	struct timeval start_time;
	struct timeval end_time;
	unsigned long long len_dw = LOOP * 64;
	float data[LENGTH];

	int sub;
	for( sub = 0; sub < LENGTH; sub += 1 ){
		data[sub] = 1.0;
	}
	
	sub = 0;

	
	gettimeofday(&start_time, NULL);
	memr();
	gettimeofday(&end_time, NULL);
	
	float unit_coeffi = 1000000.0 / (1024 * 1024);
	printf("Mem Read Test: Data Size: %f MB.\n", (double) (len_dw) / (1024 * 1024) );
	printf("Mem Read Test: Time_Consumption: %d Microseconds.\n", end_time.tv_usec - start_time.tv_usec);
	printf("Mem Read Test: Measured Value of Bandwidth: %f MB/s\n", unit_coeffi * ( ( (double)(len_dw) ) / (end_time.tv_usec - start_time.tv_usec) ) );

	return 0;
}


