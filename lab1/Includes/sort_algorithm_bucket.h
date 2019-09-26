#ifndef _SORT_ALGORITHM_BUCKET_
#define _SORT_ALGORITHM_BUCKET_

/***********************************
            Includes
***********************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <array>
#include <iterator> 
#include <set>
#include <algorithm>

/* Structure to be sent to the thread handler of bucket sort*/
struct thread_args_bucket
{
    int32_t divider;
    int32_t *input_array;
    int32_t bucket_no;
    int32_t local_thread_id;
    int32_t total_elts;
};

/***********************************
        Global Declarations
***********************************/
extern struct timespec start;
extern struct timespec end_time;
extern std::vector <std::multiset <int32_t> > B;
extern pthread_barrier_t bar;
extern pthread_mutex_t lock;
extern int32_t total_threads;
/***********************************
        Function Declarations
***********************************/
/*********************************
Function : max_value
Parameters : Input array and size of the array
Description : Function to determine the maximum value in the array
Return : returns the max value
**********************************/
int32_t max_value(int32_t [],int32_t );

/*********************************
Thread handler for buck sort
**********************************/
void* bucket_sort(void* arg);

/*********************************
Function : bucket_divider
Parameters : Input array, size of the array and number of threads
Description : Function to determine divisor to determine each bucket's range
Return : returns the divider
**********************************/
int32_t bucket_divider(int arr[], int size, int threads);

#endif