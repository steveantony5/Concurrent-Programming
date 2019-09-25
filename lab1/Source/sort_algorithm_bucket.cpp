/***********************************
            Includes
***********************************/
#include "../Includes/sort_algorithm_bucket.h"

/***********************************
        Function Definitions
***********************************/


int32_t max_value(int32_t input_array[],int32_t total_elts)
{
    int32_t max_value = 0;
    for(int m=0; m<total_elts ; m++)
    {
        if(input_array[m] > max_value)
        {
            max_value = input_array[m];
        }
    }
    //printf("Max value is %d\n",max_value);

    return  max_value;
}

int32_t bucket_divider(int32_t arr[], int32_t size, int32_t threads)
{
  //variables
  int32_t max_val =0 , bucket_total = 0 , divider_l = 0;

  bucket_total = threads;

  //find max and min
  max_val = max_value(arr, size);

  divider_l = ceil(float(max_val + 1) / bucket_total);

  //printf("Divider %d\n",divider_l);

  return divider_l;
}

// thread function for multi-threading
void* bucket_sort(void* arg)
{
    struct thread_args_bucket *args = (struct thread_args_bucket *) arg;
    int32_t id = args->local_thread_id;

    pthread_barrier_wait(&bar);
    if(id==1)
    {
        printf("Started timer\n");
        clock_gettime(CLOCK_MONOTONIC,&start);
    }
    pthread_barrier_wait(&bar);

    int divider_bucket = args->divider;

    int j;

    for(int i = 0; i < args->total_elts; i++)
    {

        j = floor( (args->input_array)[i] / divider_bucket );

        if(args->bucket_no == j)
        {
            pthread_mutex_lock(&lock);
            B[j].insert(((args->input_array)[i]));
            pthread_mutex_unlock(&lock);
        }
    }


    return NULL;
}
