#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream> 
#include <vector> // for 2D vector 
#include <string.h>

using namespace std; 

#define MAX_THREADS (20)

#ifndef HANDOVER_LOCKING
#define HANDOVER_LOCKING 
#endif


#define NO_OF_NODES (20000)

struct pair_t
{
	int64_t key;
	int64_t value;
};

std::vector<struct pair_t> vec[MAX_THREADS]; 

#if HANDOVER_LOCKING
pthread_mutex_t lock;

typedef struct BST
{
	pthread_mutex_t node_lock;
	int64_t key;
	int64_t value;
	struct BST *LC;
	struct BST *RC;
}BST;

#else
pthread_rwlock_t lock;


typedef struct BST
{
	pthread_rwlock_t node_lock;
	int64_t key;
	int64_t value;
	struct BST *LC;
	struct BST *RC;
}BST;

#endif

BST *root = NULL;

struct timespec start;
struct timespec end_time;



#if HANDOVER_LOCKING
bool insert_node(BST **root, int64_t value)
{
	pthread_mutex_lock(&lock);
	if(*root == NULL)
	{
		BST *new_child = (BST *)malloc(sizeof(BST));
		if(new_child != NULL)
		{
			new_child->key = 0;
			new_child->value = value;
			new_child->LC = NULL;
			new_child->RC = NULL;
			*root = new_child;
			pthread_mutex_unlock(&lock);
			return true;
		}
	}

	pthread_mutex_unlock(&lock);
	
	BST *cur;
	cur = *root;

	pthread_mutex_lock(&(cur->node_lock));
	//printf("cur locked 1\n");
	while(1)
	{
		if(cur->value > value)
		{
			if(cur->LC == NULL)
			{
				BST *new_child = (BST *)malloc(sizeof(BST));
				if(new_child != NULL)
				{
					new_child->key = 0;
					new_child->value = value;
					new_child->LC = NULL;
					new_child->RC = NULL;
					cur->LC = new_child;
					pthread_mutex_unlock(&(cur->node_lock));
					//printf("cur ulocked 2\n");
					return true;
				}
			}
			else
			{
				pthread_mutex_unlock(&(cur->node_lock));
				//printf("cur ulocked 3\n");
				cur = cur->LC;
				pthread_mutex_lock(&(cur->node_lock));
				//printf("cur locked 4\n");
			}
		}

		else if(cur->value < value)
		{
			if(cur->RC == NULL)
			{
				BST *new_child = (BST *)malloc(sizeof(BST));
				if(new_child != NULL)
				{
					new_child->key = 0;
					new_child->value = value;
					new_child->LC = NULL;
					new_child->RC = NULL;
					cur->RC = new_child;
					pthread_mutex_unlock(&(cur->node_lock));
					//printf("cur ulocked 5\n");
					return true;
				}
			}
			else
			{
				pthread_mutex_unlock(&(cur->node_lock));
				//printf("cur ulocked 6\n");
				cur = cur->RC;
				pthread_mutex_lock(&(cur->node_lock));
				//printf("cur locked 7\n");
			}
		}
		else if(cur->value == value)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
				if(new_child != NULL)
				{
					new_child->key = 0;
					new_child->value = value;
					new_child->LC = NULL;
					new_child->RC = NULL;
					cur->RC = new_child;
					pthread_mutex_unlock(&(cur->node_lock));
					//printf("cur ulocked 5\n");
					return true;
				}

			//printf("Abort as value already exists\n");
			pthread_mutex_unlock(&(cur->node_lock));
			//printf("cur ulocked 8\n");
			return false;

		}

	}
}

bool get_node(BST *root, int64_t key)
{
	pthread_mutex_lock(&(root->node_lock));
    while (root != NULL) 
    { 
        if (key > root->value) 
        {
        	pthread_mutex_unlock(&(root->node_lock));
            root = root->RC; 

            if(root != NULL)
           	 pthread_mutex_lock(&(root->node_lock));
        }
  
        else if (key < root->value) 
        {
        	pthread_mutex_unlock(&(root->node_lock));
            root = root->LC; 

            if(root != NULL)
            	pthread_mutex_lock(&(root->node_lock));
        }
        else
        {
        	pthread_mutex_unlock(&(root->node_lock));
            return true; 
        }
    } 
    return false; 

}

void in_order_traversal(BST *root,int thread)
{

	pthread_mutex_lock(&lock);
	if(root == NULL)
	{
		pthread_mutex_unlock(&lock);

		return;
	}
	else
	{
		pthread_mutex_unlock(&lock);
	}

	pthread_mutex_unlock(&(root->node_lock));

	if(root->LC != NULL)
		pthread_mutex_lock(&((root->LC)->node_lock));

	in_order_traversal(root->LC,thread);
	struct pair_t pair_value;
	pair_value.value = root->value;
	pair_value.key   = root->key;

	vec[thread].push_back(pair_value);

	if(root->RC != NULL)
		pthread_mutex_lock(&((root->RC)->node_lock));

	
	in_order_traversal(root->RC,thread);

}

#else



bool insert_node(BST **root, int64_t value)
{
	//printf("cur locked 1\n");
	pthread_rwlock_wrlock(&lock);
	if(*root == NULL)
	{
		BST *new_child = (BST *)malloc(sizeof(BST));
		if(new_child != NULL)
		{
			new_child->key = 0;
			new_child->value = value;
			new_child->LC = NULL;
			new_child->RC = NULL;
			*root = new_child;

			//printf("cur ulocked 2\n");
			pthread_rwlock_unlock(&lock);
			return true;
		}
	}

	//printf("cur ulocked 3\n");
	pthread_rwlock_unlock(&lock);
	
	BST *cur;
	cur = *root;

	//printf("cur locked 4 %ld\n",cur->value);
	pthread_rwlock_wrlock(&(cur->node_lock));
	
	while(1)
	{
		if(cur->value > value)
		{
			if(cur->LC == NULL)
			{
				BST *new_child = (BST *)malloc(sizeof(BST));
				if(new_child != NULL)
				{
					new_child->key = 0;
					new_child->value = value;
					new_child->LC = NULL;
					new_child->RC = NULL;
					cur->LC = new_child;

					//printf("cur ulocked 5 %ld\n",cur->value);
					pthread_rwlock_unlock(&(cur->node_lock));
					return true;
				}
			}

			else
			{
				//printf("cur ulocked 6 %ld\n",cur->value);
				pthread_rwlock_unlock(&(cur->node_lock));
				//printf("cur ulocked 3\n");
				cur = cur->LC;
				//printf("cur locked 7 %ld\n",cur->value);
				pthread_rwlock_wrlock(&(cur->node_lock));
				//printf("cur locked 4\n");
			}
		}

		else if(cur->value < value)
		{
			if(cur->RC == NULL)
			{
				BST *new_child = (BST *)malloc(sizeof(BST));
				if(new_child != NULL)
				{
					new_child->key = 0;
					new_child->value = value;
					new_child->LC = NULL;
					new_child->RC = NULL;
					cur->RC = new_child;

					//printf("cur ulocked 8 %ld\n",cur->value);
					pthread_rwlock_unlock(&(cur->node_lock));
					//printf("cur ulocked 5\n");
					return true;
				}
			}
			else
			{
				//printf("cur ulocked 9 %ld\n",cur->value);
				pthread_rwlock_unlock(&(cur->node_lock));
				//printf("cur ulocked 6\n");
				cur = cur->RC;
				//printf("cur locked 10 %ld\n",cur->value);
				pthread_rwlock_wrlock(&(cur->node_lock));
				//printf("cur locked 7\n");
			}
		}
		else if(cur->value == value)
		{
			//printf("Abort as value already exists\n");
			//printf("cur ulocked 11 %ld\n",cur->value);
			pthread_rwlock_unlock(&(cur->node_lock));
			//printf("cur ulocked 8\n");
			return false;
		}
	}
}

bool get_node(BST *root, int64_t key)
{
	pthread_rwlock_rdlock(&(root->node_lock));
    while (root != NULL) 
    { 
        if (key > root->value) 
        {
        	pthread_rwlock_unlock(&(root->node_lock));
            root = root->RC; 

            if(root != NULL)
           	 pthread_rwlock_rdlock(&(root->node_lock));
        }
  
        else if (key < root->value) 
        {
        	pthread_rwlock_unlock(&(root->node_lock));
            root = root->LC; 

            if(root != NULL)
            	pthread_rwlock_rdlock(&(root->node_lock));
        }
        else
        {
        	pthread_rwlock_unlock(&(root->node_lock));
            return true; 
        }
    } 
    return false; 

}


void in_order_traversal(BST *root,int thread)
{
	//printf("locked 1 \n");
	pthread_rwlock_rdlock(&lock);
	if(root == NULL)
	{

		//printf("ulocked 2 \n");
		pthread_rwlock_unlock(&lock);

		return;
	}
	else
	{
		//printf("ulocked 3 \n");
		pthread_rwlock_unlock(&lock);
	}

	//printf("ulocked 4 %ld\n",root->value);
	pthread_rwlock_unlock(&(root->node_lock));

	if(root->LC != NULL)
	{
		//printf("locked 5 %ld\n",root->LC->value);
		pthread_rwlock_rdlock(&((root->LC)->node_lock));
	}

	in_order_traversal(root->LC,thread);
	struct pair_t pair_value;
	pair_value.value = root->value;
	pair_value.key   = root->key;

	vec[thread].push_back(pair_value);

	if(root->RC != NULL)
	{
		//printf("locked 7 %ld\n",root->RC->value);
		pthread_rwlock_rdlock(&((root->RC)->node_lock));
	}

	in_order_traversal(root->RC,thread);

}

#endif

//Divide step of merge sort Algorithm
void* handler_insert(void* arg)
{

	printf("insert thread begin for %ld \n",pthread_self());
  int64_t key = (int64_t)arg;

   for(int64_t i=1,j = key; i< NO_OF_NODES; i++, j++)
	{
		insert_node(&root,j);
	}
	printf("insert completed for %ld \n",pthread_self());

	return NULL;
}

void* handler_insert_high_contention(void* arg)
{

	printf("insert thread begin for %ld \n",pthread_self());

   for(int64_t i=1; i< NO_OF_NODES; i++)
	{
		insert_node(&root,(i % 10));
	}
	printf("insert completed for %ld \n",pthread_self());

	return NULL;
}

void* handler_get_high_contention(void* arg)
{
	printf("get thread begin for %ld \n",pthread_self());

    for(int64_t i=1; i< NO_OF_NODES; i++)
	{
		get_node(root,(i % 10));
	}

	
	printf("get completed for %ld \n",pthread_self());

	return NULL;

}

void* handler_traverse(void* arg)
{
	printf("Traverse thread begin for %ld \n",pthread_self());
   int64_t thread = (int64_t) arg;

   #if HANDOVER_LOCKING
   pthread_mutex_lock(&(root->node_lock));

   #else
   pthread_rwlock_rdlock(&(root->node_lock));
   #endif

   in_order_traversal(root,thread);
   printf("Traverse completed for %ld \n",pthread_self());

   return NULL;

}

void* handler_get(void* arg)
{
	printf("get thread begin for %ld \n",pthread_self());
  int64_t key = (int64_t)arg;

    for(int64_t i=1,j = key; i< 1000; i++, j=j+2)
	{
		get_node(root,j);
	}

	
	printf("get completed for %ld \n",pthread_self());

	return NULL;

}


int main(int argc, char *argv[])
{
	if(HANDOVER_LOCKING)
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


	if(strcmp(argv[1],"LOW_CONTENTION") == 0)
	{
		insert_node(&root,0);
		int64_t seed_value = -300000;
		printf("Testcase for low contention\n");

		clock_gettime(CLOCK_MONOTONIC,&start);

		for(int i =0; i<insert_threads; i++)
		{
			pthread_create(&thread_id_insert[i], NULL, handler_insert, (void *)seed_value);

			seed_value += 200000;
		}

		for(int i =0; i<insert_threads; i++)
		{
			pthread_join(thread_id_insert[i], NULL);
		}



		clock_gettime(CLOCK_MONOTONIC,&end_time);

		unsigned long long elapsed_ns;
        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
        double elapsed_s = ((double)elapsed_ns)/1000000000.0;
        printf("Total Time taken for %d threads insering %d elements each(s): %lf\n\n\n",total_threads ,NO_OF_NODES,elapsed_s);

        /*******************************************/

		printf("Test case for simultaneous operation of insertion, get, range query\n");
		seed_value = 2000000;
		clock_gettime(CLOCK_MONOTONIC,&start);

		for(int i =0; i<insert_threads; i++)
		{
			pthread_create(&thread_id_insert[i], NULL, handler_insert, (void *)seed_value);

			seed_value += 200000;
		}

		int64_t seed_get = -100000;
		for(int i =0; i<get_threads; i++)
		{
			pthread_create(&thread_id_get[i], NULL, handler_get, (void *)seed_get);
			seed_get += 200000;
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

        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
        elapsed_s = ((double)elapsed_ns)/1000000000.0;
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

	else if(strcmp(argv[1],"HIGH_CONTENTION") == 0)
	{
		insert_node(&root,NO_OF_NODES+1);
		int64_t seed_value = NO_OF_NODES;
		printf("Testcase for high contention\n");


		for(int64_t i=seed_value; i >= 0; i--)
		{
			insert_node(&root,i);
		}

		printf("Populated a skewed BST to test high contention\n");

		clock_gettime(CLOCK_MONOTONIC,&start);

		for(int i =0; i<get_threads; i++)
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
        
        printf("Total Time taken for %d threads insering %d elements each(s): %lf\n\n\n",total_threads ,NO_OF_NODES,elapsed_s);

        /*******************************************/

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

        elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
        elapsed_s = ((double)elapsed_ns)/1000000000.0;
        printf("Total Time taken for all operations happening concurrently (s): %lf\n",elapsed_s);


	}


	else
	{
		printf("Please pass command line argument\n");
		printf("LOW_CONTENTION\nHIGH_CONTENTION\n");
	}

	return 0;

}


