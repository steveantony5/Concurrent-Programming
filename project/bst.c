#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
typedef struct BST
{
	pthread_mutex_t node_lock;
	int64_t key;
	int64_t value;
	struct BST *LC;
	struct BST *RC;
}BST;

BST *root = NULL;

pthread_mutex_t lock;

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
			printf("Abort as value already exists\n");
			pthread_mutex_unlock(&(cur->node_lock));
			//printf("cur ulocked 8\n");
			return false;

		}

	}
}

void* in_order_traversal(BST *root)
{

	pthread_mutex_lock(&lock);
	if(root == NULL)
	{
		pthread_mutex_unlock(&lock);

		return NULL;
	}
	else
	{
		pthread_mutex_unlock(&lock);
	}

	pthread_mutex_unlock(&(root->node_lock));

	if(root->LC != NULL)
		pthread_mutex_lock(&((root->LC)->node_lock));

	in_order_traversal(root->LC);

	printf("%ld ",root->value);

	if(root->LC != NULL)
		pthread_mutex_unlock(&((root->LC)->node_lock));

	if(root->RC != NULL)
		pthread_mutex_lock(&((root->RC)->node_lock));

	
	in_order_traversal(root->RC);


}

//Divide step of merge sort Algorithm
void* handler1(void* arg)
{
   printf("thread1\n");
   for(int64_t i=1; i< 50000; i = i+2)
	{
		insert_node(&root,i);
	}
	printf("insert comp 1 \n");

}

//Divide step of merge sort Algorithm
void* handler2(void* arg)
{
   printf("thread2\n");
   for(int64_t i=0; i< 50000; i=i+2)
	{
		insert_node(&root,i);
	}
	printf("insert comp 2 \n");

}
//Divide step of merge sort Algorithm
void* handler3(void* arg)
{
	printf("Traversal\n");

	pthread_mutex_lock(&(root->node_lock));
   in_order_traversal(root);

}

int64_t main()
{
	pthread_t thread_id[4];
	
	int64_t a[] = {-23};
	insert_node(&root, a[0]);
	printf("root node %ld\n",root->value);

	pthread_create(&thread_id[0], NULL, handler1, NULL);
	pthread_create(&thread_id[1], NULL, handler2, NULL);

	sleep(4);
	pthread_create(&thread_id[2], NULL, handler3, NULL);

	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[2], NULL);

	printf("End\n");

}


