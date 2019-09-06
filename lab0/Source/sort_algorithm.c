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
void merge_sort(int32_t input[], int32_t low, int32_t high, int32_t elts)
{
    if(low < high)
    {

        //get the middle index
        int32_t middle = low + (high - low) / 2 ;


        merge_sort(input, low,middle, elts);
        merge_sort(input, middle+1, high, elts);


        merge(input, low, middle, high, elts);
    }
}

//Conquer and merge step of merge sort Algorithm
int merge(int32_t input[], int32_t low, int32_t middle, int32_t high, int32_t elts)
{
    // temporary array
    temp = malloc(elts * sizeof(int32_t));

    if(temp == NULL)
    {
        printf("Couldn't malloc\n");
        return -1;
    }

    //copying the divided input array to a temporary array
    for(int32_t i=0; i<=high;i++)
    {
        temp[i] = input[i];
    }

    int32_t i = low;
    int32_t j = middle + 1;
    int32_t k = low;

    while((i <= middle) && (j <= high))
    {
        if(temp[i] <= temp[j])
        {
            input[k] = temp[i];
            i++;
        }
        else
        {
            input[k] = temp[j];
            j++;
        }
        k++;
    }

    while(i <= middle)
    {
        input[k] = temp[i];
        k++;
        i++;
    }

    return 0;
}