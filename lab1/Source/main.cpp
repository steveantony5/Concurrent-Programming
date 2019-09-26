/***********************************
            Includes
***********************************/
#include "../Includes/main.h"

#define DEBUG 0
using namespace std;

/**********************************
          GLOBALS 
**********************************/
//Bucket vector of multisets
vector <multiset <int32_t> > B;

pthread_barrier_t bar;
struct timespec start;
struct timespec end_time;

pthread_mutex_t lock;

int total_threads = 1;
/**********************************
           Locals
**********************************/

static int32_t total_elts;
/**********************************
        Main Function
**********************************/
int main(int argc, char *argv[])
{
    int c;
    FILE *input, *output;

    size_t len = 0;
    char outputfilename[20];
    char inputfilename[20];
    int arg_filename = 0;
    char algos[10];
    int bucket = 0;
    int fj = 0;

    char buffer[20];

    if(argv[1]== NULL)
    {
        printf("Please provide arguments\n");
        printf("./mysort [Sourcefilename] -o [outputfilename]\nSource filename mandatory\n");
        exit(1);
    }

    strcpy(inputfilename,argv[1]);

    static struct option long_options[] =
    {
      /* These options set a flag. */
      {"name",no_argument,0,'n'},
      /* These options don’t set a flag.
         We distinguish them by their indices. */
      {"object",optional_argument,0, 'o'},
      {"threads", optional_argument,0,'t'},
      {"algs", required_argument,0,'a'},
      {0, 0, 0, 0}
    };

    //parsing command line arguments
    while ((c = getopt_long (argc, argv, "n:o:t:a:", long_options, 0)) != -1)
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

            case 'a':
                strcpy(algos,optarg);
                printf("algorithm = %s\n",algos);
                if(strcmp(algos,"bucket") == 0)
                {
                    bucket = 1;
                }
                else if (strcmp(algos,"fj") == 0)
                {
                    fj = 1;
                }
                else
                {
                    printf("Choose a sorting algorithm\n");
                    exit(1);
                }

                break;
            default:
                printf("No arguments provided\n");
                exit(1);
        }
    }

    printf("Threads = %d\n",total_threads);


    //initializing the barrier
    pthread_barrier_init(&bar, NULL, total_threads);

    printf ("inputfilename = %s\n",inputfilename);

    input = fopen(inputfilename,"r");
    if(input == NULL)
    {
        printf("Couldn't open file as it doesn't exists\n");
        exit(1);
    }

    struct stat st;
    if(stat(inputfilename, &st) != 0)
    {
        return 0;
    }

    char * line = NULL;
    //find the number of integers in the input file
    while((getline(&line, &len, input)) != -1)
    {
        total_elts++;

    }
    free(line);

    //reset the file pointer to the begining
    fseek(input, 0, SEEK_SET);

    //create an input array to store contents from the file
    int32_t input_array[total_elts];

    int32_t index = 0;

    char * line_1 = NULL;
    //store the elements in an integer array
    while((getline(&line_1, &len, input)) != -1)
    {
        input_array[index] = atoi(line_1);
        index++;
    }
    free(line_1);
    fclose(input);

    printf("Total elements %d\n",total_elts);


    if(fj == 1)
    {
        struct thread_task task[total_threads];

        printf("FORK JOIN MODEL : Merge sort\n");
        for (int i = 0; i < total_threads; i++)
        {
            // calculating low and high
            int32_t low = i * (total_elts / total_threads);
            int32_t high;

            if(i == (total_threads - 1))
            {
                high = (total_elts - 1);
            }
            else
            {
                high = (i + 1) * (total_elts / total_threads) - 1;
            }

            task[i].tsk_low = low;
            task[i].tsk_high = high;
            task[i].tsk_local_id = i;
            task[i].input_array = input_array;

            #if DEBUG
            printf("task[%d] %d %d\n",i,task[i].tsk_low,task[i].tsk_high);
            #endif

        }


        /****************************************************
                    Creating threads
        ****************************************************/
        pthread_t threads[total_threads];

        for (int i = 0; i < total_threads; i++)
        {
            task[i].local_thread_id = i+1;
            if(!(pthread_create(&threads[i], NULL, merge_Sort, (void*)&task[i]) == 0))
            {
                printf("Error on creating threads\n");
                exit(1);
            }
            #if DEBUG
            printf("creating thread %d\n",i);
            #endif
        }

        // joining all threads
        for (int i = 0; i < total_threads; i++)
        {
            pthread_join(threads[i], NULL);
            #if DEBUG
            printf("Joining thread %d\n",i);
            #endif
        }


        for (int i = 1; i < total_threads; i++)
        {

            int mid = task[i-1].tsk_high ;

            #if DEBUG
            printf("merging mid %d high %d\n", mid,task[i].tsk_high);
            #endif

            merge(task[i].input_array,0, mid, task[i].tsk_high);

        }
        clock_gettime(CLOCK_MONOTONIC,&end_time);

        unsigned long long elapsed_ns;
        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
        printf("Elapsed (ns): %llu\n",elapsed_ns);
        double elapsed_s = ((double)elapsed_ns)/1000000000.0;
        printf("Elapsed (s): %lf\n",elapsed_s);

    }

    if(bucket == 1)
    {
        printf("LOCKING BUCKET SORTING\n");



        struct thread_args_bucket arg[total_threads];
        /****************************************************
                Find the maximum element in the input
        ****************************************************/

        int32_t divider = bucket_divider(input_array,total_elts,total_threads);

        //resizing the bucket based on the number of threads
        B.resize(total_threads);

        /****************************************************
                    Creating threads
        ****************************************************/
        pthread_t threads[total_threads];
        int32_t min_index, max_index, range;

        for (int32_t i = 0; i < total_threads; i++)
        {
            arg[i].divider = divider;
            arg[i].input_array = input_array;
            arg[i].local_thread_id = i+1;
            arg[i].total_elts = total_elts;


            range = (total_elts / total_threads);
            int id = i+1;

            if(id != (total_threads))
            {
              

              min_index = ((id -1) * (range)) ;
              max_index = min_index + range -1;

            }
            else
            {

              min_index = ((id -1) * (range)) ;
              max_index = total_elts -1;

            }
            arg[i].low_index = min_index;
            arg[i].high_index = max_index;


            if(!(pthread_create(&threads[i], NULL, bucket_sort, (void*)&arg[i]) == 0))
            {
                printf("Error on creating threads\n");
                exit(1);
            }
            
            #if DEBUG
            printf("creating thread %d\n",i);
            #endif

        }

        // joining all threads
        for (int32_t i = 0; i < total_threads; i++)
        {
            pthread_join(threads[i], NULL);
            
            #if DEBUG
            printf("Joining thread %d\n",i);
            #endif
        }

       int32_t k=0;
       int32_t bucket_elts = B.size();

       //coping the elements from bucket back to input array
        for(int32_t i =0; i< bucket_elts; i++)
        {
            for(std::multiset<int32_t>::iterator j=B[i].begin();j != B[i].end();++j)
            {

                input_array[k] = *j;
                k++;
            }
        }

        clock_gettime(CLOCK_MONOTONIC,&end_time);

        unsigned long long elapsed_ns;
        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
        printf("Elapsed (ns): %llu\n",elapsed_ns);
        double elapsed_s = ((double)elapsed_ns)/1000000000.0;
        printf("Elapsed (s): %lf\n",elapsed_s);

    }


    //destroy the barrier
    pthread_barrier_destroy(&bar);

    printf("**********Sorted array*****************\n");

    //if output file name is provided as command line argument
    if(arg_filename == 1)
    {
        output = fopen(outputfilename,"w");
        if(output == NULL)
        {
            printf("Couldn't open file\n");
            return -1;
        }
    }

    for(int32_t j=0; j< total_elts; j++)
    {
        //if output file is provided, store the sorted integers in the output file
        if(arg_filename == 1)
        {
            memset(buffer,'\0',sizeof(buffer));
            sprintf(buffer,"%d\n",input_array[j]);
            fwrite(buffer,1, strlen(buffer),output);
        }
        else
        {
            //print the sorted integers on the terminal if output file is not provided
            printf("%d\n", input_array[j]);
        }

    }

    if(arg_filename == 1)
    {
        printf("Sorted and Stored in %s \n",outputfilename);
        fclose(output);
    }
}




