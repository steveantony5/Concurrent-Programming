/***********************************
            Includes
***********************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>


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
int merge(int32_t arr[], int32_t low, int32_t middle, int32_t high);
/**********************************
Function    : merge
Description : Performs Conquer and merge step of Merge Sort Algorithm
Parameters  : input - Input array of unsorted integers
			  low   - Lowest index of the array
			  high  - Highest index of the array
			  elts  - Total number of elements to be sorted
return      : error/success
***********************************/
void merge_sort(int32_t input[], int32_t low, int32_t high);