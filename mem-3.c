#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sched.h>

#include "memr.h"

#define NUM_TRD 4 
#define LENGTH 3600000000
#define CACHE_LINE 64
#define ALLOC  3610000000

typedef struct
{
	float* mem_page;
	int cpu_num;
}TRD_Param;



void *page_alloc( size_t size )
{
	void* mem_alloc_ptr = (void*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if( mem_alloc_ptr == (void*)-1 ){
		fprintf(stderr, "Mem allocation FAILED!\n");
		return NULL;
	}
	else{
		;
	}
	return mem_alloc_ptr;
}


void* thread_rtin(void* param)
{
	int cpu = ((TRD_Param*)param)->cpu_num;
	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);
	CPU_SET(cpu, &cpu_set);

	if( pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set) != 0 ){
		fprintf(stderr, "Error: cpu[%d] bindind failed.\n", cpu);
		exit(0);
	}
	else{
		;
	}

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
	
	
	TRD_Param* param = (TRD_Param*)malloc(sizeof(TRD_Param));
	param->cpu_num = 0;
	param->mem_page = src_data;

	gettimeofday(&start_time, NULL);
	
	for( i = 0; i < NUM_TRD && param->cpu_num < NUM_TRD; i++, param->cpu_num += 1 ){
		pthread_create(&tids[i], NULL, thread_rtin, (void *)param);
	}
	
	for( i = 0, param->cpu_num = 0; i < NUM_TRD; i++, param->cpu_num += 1 ){
		pthread_join(tids[i], NULL);
	}

	gettimeofday(&end_time, NULL);

	free(tids);
	free(param);

	int mmap_free = munmap((void*)src_data, ALLOC * sizeof(float));
	if( mmap_free == -1 ){
		fprintf(stderr, "Mmap free FAILED!\n");
	}
	else{
		printf("Mapping mem freed!\n");
	}	
	

	double data_rd_size =  ((double)(LENGTH * sizeof(float))) / (1024 * 1024);
	double run_time_consumption =  ((double) ((end_time.tv_usec - start_time.tv_usec) * 1e-6 + (end_time.tv_sec - start_time.tv_sec)));
	printf("Mem Read Test: Data Size: %f MB.\n", data_rd_size);
	printf("time_consumption = %f s\n", run_time_consumption);
	printf("perf=%f\n", data_rd_size / run_time_consumption);

	return 0;
}


