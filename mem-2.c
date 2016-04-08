#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <pthread.h>
#include "memr.h"

#define NUM_TRD 2 
#define LENGTH 1600000000
#define CACHE_LINE 64
#define ALLOC  1700000000

void *page_alloc( size_t size )
{
	void* mem_alloc_ptr = (void*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if( mem_alloc_ptr == (void*)-1 ){
		fprintf(stderr, "Mem allocation FAILED!\n");
		return NULL;
	}
	return mem_alloc_ptr;
}


void* thread_rtin(void* data)
{
	memr();
	return NULL;
}


int main( void )
{
	struct timeval start_time;
	struct timeval end_time;
	int i;

	float *src_data = (float*)page_alloc(ALLOC * sizeof(float));
	pthread_t* tids = (pthread_t*)calloc(NUM_TRD, sizeof(pthread_t));
	
	int val_sep = LENGTH / NUM_TRD;
	int size_sep = 0;


	gettimeofday(&start_time, NULL);
	for( i = 0; i < NUM_TRD; i++, size_sep += val_sep ){
		pthread_create(&tids[i], NULL, thread_rtin, (void *)(src_data + size_sep));
	}
	
	for( i = 0; i < NUM_TRD; i++ ){
		pthread_join(tids[i], NULL);
	}
	gettimeofday(&end_time, NULL);

	free(tids);

	double data_rd_size =  ((double)(LENGTH * sizeof(float))) / (1024 * 1024);
	double run_time_consumption =  ((double) ((end_time.tv_usec - start_time.tv_usec) * 1e-6 + (end_time.tv_sec - start_time.tv_sec)));
	printf("Mem Read Test: Data Size: %f MB.\n", ((double)(LENGTH * sizeof(float))) / (1024 * 1024) );
	printf("time_consumption = %f s\n", run_time_consumption);
	printf("perf=%f\n", data_rd_size / run_time_consumption);

	return 0;
}


