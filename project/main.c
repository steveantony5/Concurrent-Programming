#include "bst.h"

struct timespec start;
struct timespec end_time;

int main(int argc, char *argv[])
{

	if(MUTEX_LOCKING)
	{
		printf("\n\nHANDOVER LOCKING\n\n");
	}
	else
	{
		printf("\n\nRW LOCKING\n\n");
	}
	pthread_t thread_id_insert[MAX_THREADS];

	pthread_t thread_id_traverse[MAX_THREADS];

	pthread_t thread_id_get[MAX_THREADS];

	uint8_t total_threads = atoi(argv[2]);
	uint8_t insert_threads = total_threads;
	uint8_t traverse_threads = total_threads;
	uint8_t get_threads = total_threads;

	NO_OF_NODES = atoi(argv[3]);

	int testcase = atoi(argv[4]);


	if(strcmp(argv[1],"LOW_CONTENTION") == 0)
	{

		if(testcase == 1)
		{
			insert_node(NULL,(NO_OF_NODES*total_threads)/2,rand());
			int64_t seed_value = NO_OF_NODES;
			printf("Testcase for low contention\n");

			clock_gettime(CLOCK_MONOTONIC,&start);

			for(int i =0; i<insert_threads; i++)
			{
				pthread_create(&thread_id_insert[i], NULL, handler_insert, (void *)seed_value);

				seed_value += NO_OF_NODES;
			}

			for(int i =0; i<insert_threads; i++)
			{
				pthread_join(thread_id_insert[i], NULL);
			}



			clock_gettime(CLOCK_MONOTONIC,&end_time);

			unsigned long long elapsed_ns;
	        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	        double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	        printf("Total Time taken for %d threads insering %ld elements each(s): %lf\n\n\n",total_threads ,NO_OF_NODES,elapsed_s);
	    }

	    if(testcase == 2)
	    {

	        root = (BST *)malloc(sizeof(BST));
			if(root != NULL)
			{
				root->key = (NO_OF_NODES*total_threads)/2;
				root->value = rand();
				root->LC = NULL;
				root->RC = NULL;
			}


			printf("Test case for simultaneous operation of insertion, get, range query\n");
			int64_t seed_value = NO_OF_NODES;
			clock_gettime(CLOCK_MONOTONIC,&start);

			for(int i =0; i<insert_threads; i++)
			{
				pthread_create(&thread_id_insert[i], NULL, handler_insert, (void *)seed_value);

				seed_value += NO_OF_NODES;
			}

			int64_t seed_get = NO_OF_NODES;
			for(int i =0; i<get_threads; i++)
			{
				pthread_create(&thread_id_get[i], NULL, handler_get, (void *)seed_get);
				seed_get += NO_OF_NODES;
			}
			
			for(int64_t i =0; i<traverse_threads; i++)
			{
				pthread_create(&thread_id_traverse[i], NULL, handler_traverse, (void *)i);	
			}

			for(int i =0; i<insert_threads; i++)
			{
				pthread_join(thread_id_insert[i], NULL);
			}

			for(int i =0; i<traverse_threads; i++)
			{
				pthread_join(thread_id_traverse[i], NULL);
				
			}

			for(int i =0; i<get_threads; i++)
			{
				pthread_join(thread_id_get[i], NULL);
			}

			clock_gettime(CLOCK_MONOTONIC,&end_time);

	        unsigned long long elapsed_ns;
	        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	        double elapsed_s = ((double)elapsed_ns)/1000000000.0;

	        printf("Total Time taken for all operations happening concurrently (s): %lf\n",elapsed_s);


		   /*	
		   printf("printing\n");

			for(int i =0; i<traverse_threads;i++)
			{
				printf("Vector[%d]\n",i);
				for(int64_t j=0; j<vec[i].size();j++)
				{
					printf("%ld ",vec[i][j].value);
				}
			}*/		
		}							


	}

	else if(strcmp(argv[1],"HIGH_CONTENTION") == 0)
	{
		if(testcase == 1)
		{
			insert_node(NULL,NO_OF_NODES+1,rand());
			int64_t seed_value = NO_OF_NODES;
			printf("Testcase for high contention\n");


			for(int64_t i=seed_value; i >= 0; i--)
			{
				insert_node(NULL,i,rand());
			}

			printf("Populated a skewed BST to test high contention\n");

			clock_gettime(CLOCK_MONOTONIC,&start);

			for(int i =0; i<insert_threads; i++)
			{
				pthread_create(&thread_id_insert[i], NULL, handler_insert_high_contention, NULL);
			}

			for(int i =0; i<insert_threads; i++)
			{
				pthread_join(thread_id_insert[i], NULL);
			}

			clock_gettime(CLOCK_MONOTONIC,&end_time);

			unsigned long long elapsed_ns;
	        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	        double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	        
	        printf("Total Time taken for %d threads insering %ld elements each(s): %lf\n\n\n",total_threads ,NO_OF_NODES,elapsed_s);
	    }

        /*******************************************/

        if(testcase == 2)
        {

        	insert_node(NULL,NO_OF_NODES+1,rand());
			int64_t seed_value = NO_OF_NODES;
			printf("Testcase for high contention\n");


			for(int64_t i=seed_value; i >= 0; i--)
			{
				insert_node(NULL,i,rand());
			}

			printf("Populated a skewed BST to test high contention\n");

	        printf("Test case for simultaneous operation of insertion, get, range query\n");
			clock_gettime(CLOCK_MONOTONIC,&start);

			for(int i =0; i<insert_threads; i++)
			{
				pthread_create(&thread_id_insert[i], NULL, handler_insert_high_contention, NULL);

			}

			for(int i =0; i<get_threads; i++)
			{
				pthread_create(&thread_id_get[i], NULL, handler_get_high_contention, NULL);
			}
			
			for(int64_t i =0; i<traverse_threads; i++)
			{
				pthread_create(&thread_id_traverse[i], NULL, handler_traverse, (void *)i);	
			}

			for(int i =0; i<insert_threads; i++)
			{
				pthread_join(thread_id_insert[i], NULL);
			}

			for(int i =0; i<traverse_threads; i++)
			{
				pthread_join(thread_id_traverse[i], NULL);
				
			}

			for(int i =0; i<get_threads; i++)
			{
				pthread_join(thread_id_get[i], NULL);
			}

			clock_gettime(CLOCK_MONOTONIC,&end_time);

	        unsigned long long elapsed_ns;
	        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	        double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	        printf("Total Time taken for all operations happening concurrently (s): %lf\n",elapsed_s);

	    }
	}


	else
	{
		printf("Please pass command line argument\n");
		printf("LOW_CONTENTION\nHIGH_CONTENTION\n");
	}

	return 0;

}


