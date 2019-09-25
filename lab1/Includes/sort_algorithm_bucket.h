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


struct thread_args_bucket
{
    int32_t divider;
    int32_t *input_array;
    int32_t bucket_no;
    int32_t local_thread_id;
    int32_t total_elts;
};


extern struct timespec start;
extern struct timespec end_time;
extern std::vector <std::multiset <int32_t> > B;


extern pthread_barrier_t bar;
extern pthread_mutex_t lock;

/***********************************
        Function Declarations
***********************************/



int32_t max_value(int32_t [],int32_t );
int32_t bucket_creation(int [], int , int );
void* bucket_sort(void* arg);
int32_t bucket_divider(int arr[], int size, int threads);

#endif