#ifndef _SORT_ALGORITHM_FJ_
#define _SORT_ALGORITHM_FJ_

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
#include <getopt.h>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <array>
#include <iterator> 
#include <set>
#include <algorithm>

struct thread_task
{
    int32_t *input_array;
    int32_t tsk_local_id;
    int32_t tsk_low;
    int32_t tsk_high;
    int32_t local_thread_id;
};


extern struct timespec start;
extern struct timespec end_time;
extern std::vector <std::multiset <int32_t> > B;


extern pthread_barrier_t bar;

/***********************************
        Function Declarations
***********************************/

/**********************************
Function    : merge_sort
Description : Performs Divide step of Merge Sort Algorithm
Parameters  : input - Input array of unsorted integers
			  low   - Lowest index of the array
			  high  - Highest index of the array
			  elts  - Total number of elements to be sorted
return      : NULL
***********************************/
void merge_sort(int32_t input[], int32_t low, int32_t high);

/**********************************
Function    : merge
Description : Performs Conquer and merge step of Merge Sort Algorithm
Parameters  : input - Input array of unsorted integers
			  low   - Lowest index of the array
			  high  - Highest index of the array
			  elts  - Total number of elements to be sorted
return      : error/success
***********************************/
int merge(int32_t input[], int32_t low, int32_t middle, int32_t high);

void* merge_Sort(void* arg) ;

#endif