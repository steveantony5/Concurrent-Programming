/***********************************
  Reference: https://www.dyclassroom.com/sorting-algorithm/bucket-sort
***********************************/


/***********************************
            Includes
***********************************/
#include "../Includes/sort_algorithm_bucket.h"

/***********************************
        Function Definitions
***********************************/
extern int lock_number, unlock_number;
extern bool MCS_flag;
extern bool lock_set;

const int NUM_FUNCS = 7;
void (*funcs[NUM_FUNCS])(void)  = {
    TAS_lock, //0
    Test_TAS_lock, //1
    acquire_ticketlock, //2
    pthread_lock, //3
    release_ticketlock, //4
    unlock, //5
    pthread_unlock //6
};

const char* func_names[NUM_FUNCS] = {
    "TAS_lock", //0
    "Test_TAS_lock", //1
    "acquire_ticketlock", //2
    "pthread_lock", //3
    "release_ticketlock", //4
    "unlock", //5
    "pthread_unlock" //6
};
std::atomic<MCS_node*> tail{NULL};


// thread function for multi-threading
void* bucket_sort(void* arg)
{
    MCS local_obj;
    MCS_node node;

    struct thread_args_bucket *args = (struct thread_args_bucket *) arg;
    int32_t id = args->local_thread_id;

    //getting the start time of algorithm execution
    pthread_barrier_wait(&bar);
    if(id==1)
    {
        printf("Started timer\n");
        clock_gettime(CLOCK_MONOTONIC,&start);
    }

    //poping the values from the structure argument
    int32_t divider_bucket = args->divider;
    int32_t min_index = args->low_index;
    int32_t max_index = args->high_index;

    //inserting the elements into bucket
    int j = 0;
    for(int i = min_index; i <= (max_index); i++)
    {

        j = floor( (args->input_array)[i] / divider_bucket );

        if(MCS_flag == true)
        {
            //printf("lock mcs\n");
            local_obj.mcs_acquire(&node);
        }
        else if(lock_set == true)
        {
            //printf("lock\n");
            void (*lock_type)() = funcs[lock_number];
            lock_type();
        }
        //pthread_lock();

        B[j].insert(((args->input_array)[i]));

        //pthread_unlock();
        
        
        if(MCS_flag == true)
        {
            //printf("unlock mcs\n");
            local_obj.mcs_release(&node);
        }
        else if(lock_set == true)
        {
            //printf("unlock lock\n");
            void (*lock_type)() = funcs[unlock_number];
            lock_type();
        }

    }

    return NULL;
}

/*function to find max value in an array*/
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

    return  max_value;
}

/*Function to find bucket divider*/
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