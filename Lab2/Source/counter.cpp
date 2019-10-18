/***********************************
            Includes
***********************************/
#include "../Includes/locks.h"

#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include <getopt.h>
#include <string.h>

using namespace std;

/**********************************
          GLOBALS
**********************************/
int total_threads = 0;
int iterations = 1;
int count;
char lock_type_arg[10];
char bar_type_arg[10];
int lock_number, unlock_number, barrier_number;
bool bar_set = false;
bool lock_set = false;
int8_t arg_filename = 0;

struct timespec start;
struct timespec end_time;
bool MCS_flag = false;
pthread_barrier_t bar;

/*
Thread arguments
*/
typedef struct thread_data
{
	pthread_t tid;
	int my_thread_id;
    
}TM;

/*Function pointer for locks*/
const int NUM_FUNCS = 11;
void (*funcs[NUM_FUNCS])(void)  = {
    TAS_lock, //0
    Test_TAS_lock, //1
    acquire_ticketlock, //2
    pthread_lock, //3
    release_ticketlock, //4
    unlock, //5
    pthread_unlock, //6
    sense_bar,//7
    pthread_bar //8
};

const char* func_names[NUM_FUNCS] = {
    "TAS_lock", //0
    "Test_TAS_lock", //1
    "acquire_ticketlock", //2
    "pthread_lock", //3
    "release_ticketlock", //4
    "unlock", //5
    "pthread_unlock", //6
    "sense_bar",//7
    "pthread_bar" //8
};

std::atomic<MCS_node*> tail{NULL};
static void *thread_callback(void * );
bool thread_create( void *(*thread_entry)(void*), TM *);

int main(int argc, char *argv[])
{
    int c;
    char outputfilename[20];

    if(argv[1]== NULL)
    {
        printf("Please provide arguments\n");
        printf("./counter [-t NUM_THREADS] [-i=NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] -o [outputfilename]\n");
        exit(1);
    }

    static struct option long_options[] =
    {
      /* These options set a flag. */
      {"name",no_argument,0,'n'},
      {"object",optional_argument,0, 'o'},
      /* These options don’t set a flag.
         We distinguish them by their indices. */
      {"threads", optional_argument,0,'t'},
      {"iterations", optional_argument,0,'i'},
      {"lock", optional_argument,0,'l'},
      {"bar", optional_argument,0,'b'},
      {0, 0, 0, 0}
    };

    //parsing command line arguments
    while ((c = getopt_long (argc, argv, "n:t:i:l:b:o:", long_options, 0)) != -1)
    {
        switch (c)
        {
            case 'n':
                printf("option -n\n");
                printf("\nSteve Antony Xavier Kennedy\n");
                exit(1);
                break;

            case 'o':
                strcpy(outputfilename,optarg);
                printf ("outputfilename = %s\n",outputfilename);
                arg_filename = 1;
                break;

            case 't':
                total_threads = atoi(optarg);
                break;

            case 'i':
            	iterations = atoi(optarg);
                break;

            case 'l':
                lock_set = true;
            	strcpy(lock_type_arg,optarg);
                printf("Args %s\n",lock_type_arg);
                if(strcmp(lock_type_arg,"tas") == 0)
                {
                    printf("TAS lock\n");
                    lock_number = 0;
                    unlock_number = 5;
                }    
                else if(strcmp(lock_type_arg,"ttas") == 0)
                {
                    printf("TTAS lock\n");
                    lock_number = 1;
                    unlock_number = 5;
                }    
                else if(strcmp(lock_type_arg,"ticket") == 0)
                {
                    printf("Ticket lock\n");
                    lock_number = 2;
                    unlock_number = 4;
                }    
                else if(strcmp(lock_type_arg,"mcs") == 0)
                {
                    printf("MCS lock\n");
                    MCS_flag = true;
                }    
                else if(strcmp(lock_type_arg,"pthread") == 0)
                {
                    printf("Mutex lock\n");
                    lock_number = 3;
                    unlock_number = 6;
                }    


                break;

             case 'b':
                bar_set = true;

                strcpy(bar_type_arg,optarg);
                printf("Args %s\n",bar_type_arg);
                if(strcmp(bar_type_arg,"sense") == 0)
                {
                    printf("Sense reversal barrier\n");
                    barrier_number = 7;
                }    
                else if(strcmp(bar_type_arg,"pthread") == 0)
                {
                    printf("Pthread barrier\n");
                    barrier_number = 8;
                } 
                

                break;

                
            default:
                printf("No arguments provided\n");
                exit(1);
        }
    }


    if((bar_set == true) && (lock_set == true))
    {
        printf("Setting bar and lock is invalid\n");
        exit(0);
    }

    if(bar_set)
        printf("Chosen bar\n");
    if(lock_set)
        printf("Chosen locks\n");

    printf("Threads = %d\n",total_threads);
    printf("Iterations = %d\n",iterations);

    pthread_barrier_init(&bar, NULL, total_threads);

    TM data[total_threads];

    for(int i =0; i< total_threads; i++)
    {
		data[i].my_thread_id = i;
		thread_create(thread_callback,&data[i]);
    }

    for(int i =0; i< total_threads; i++)
    {
        pthread_join(data[i].tid , NULL);
    }



    clock_gettime(CLOCK_MONOTONIC,&end_time);


    //if output file name is provided as command line argument
    if(arg_filename == 1)
    {
        FILE *output;
        char buffer[10000];
        output = fopen(outputfilename,"w");
        if(output == NULL)
        {
            printf("Couldn't open file\n");
            return -1;
        }

        memset(buffer,'\0',sizeof(buffer));
        sprintf(buffer,"%d\n",count);
        fwrite(buffer,1, strlen(buffer),output);
        fclose(output);
        printf("Output stored in file %s\n",outputfilename);
        
    }
    else
    {
        printf("Count %d\n",count);
    }

    unsigned long long elapsed_ns;
    elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);
    double elapsed_s = ((double)elapsed_ns)/1000000000.0;
    printf("Elapsed (s): %lf\n",elapsed_s);

    printf("Exiting\n");
    
}


/*Thread creation*/
bool thread_create( void *(*thread_entry)(void*), TM *data)
{

	pthread_attr_t attr;
	pthread_attr_init(&attr);
    /*Spawning the new thread*/
	if( pthread_create(&(data->tid), &attr, thread_entry, (void*)data) != 0 )
	{
        perror("thread creation failed");
        return false;
	}

    return true;
}

/**
 * Callback with void signature for pthread_create
 */
static void *thread_callback(void * t)
{
    MCS local_obj;
    MCS_node node;

	TM *thread_data = (TM *)t;

    pthread_barrier_wait(&bar);
    if((thread_data->my_thread_id)==1)
    {
        printf("Started timer\n");
        clock_gettime(CLOCK_MONOTONIC,&start);
    }

	for(int i = 0; i<(iterations * total_threads); i++)
	{
		if(i%total_threads == (thread_data->my_thread_id))
		{

            /*lock*/
            if(MCS_flag == true)
            {
                local_obj.mcs_acquire(&node);
            }
            else if(lock_set == true)
            {
                void (*lock_type)() = funcs[lock_number];
                lock_type();  
            }
			
            count++;

            /*unlock*/
            if(MCS_flag == true)
            {
                local_obj.mcs_release(&node);
            }
            else if(lock_set == true)
            {
                void (*lock_type)() = funcs[unlock_number];
                lock_type();
            }
		}

        /*barrier*/
        if(bar_set == true)
        {
            void (*lock_type)() = funcs[barrier_number];
            lock_type();
        }
        
	}   

	
	return NULL;
}
