/**********************************
    Learned Merge sort from https://www.youtube.com/watch?v=7LN9z140U90
    and implemented it
***********************************
            Includes
***********************************/
#include "sort_algorithm.h"

int32_t *temp;
/***********************************
        Function Definitions
***********************************/

//Divide step of merge sort Algorithm
void merge_sort(int32_t input[], int32_t low, int32_t high)
{
    if(low < high)
    {

        //get the middle index
        int32_t middle = low + (high - low) / 2 ;


        merge_sort(input, low,middle);
        merge_sort(input, middle+1, high);


        merge(input, low, middle, high);
    }
}
//Conquer and merge step of merge sort Algorithm
int merge(int32_t arr[], int32_t low, int32_t middle, int32_t high)
{
    int i, j, k;
    int n1 = middle - low + 1;
    int n2 =  high - middle;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[low + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = low; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    return 1;
}

