#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream> 
#include <vector> // for 2D vector 
#include <string.h>
#include <limits.h>

using namespace std; 

#define MAX_THREADS (100)

#ifndef MUTEX_LOCKING
#define MUTEX_LOCKING 
#endif

#if MUTEX_LOCKING


typedef struct BST
{
	pthread_mutex_t node_lock;
	int64_t key;
	int64_t value;
	struct BST *LC;
	struct BST *RC;
}BST;

#else



typedef struct BST
{
	pthread_rwlock_t node_lock;
	int64_t key;
	int64_t value;
	struct BST *LC;
	struct BST *RC;
}BST;

#endif

bool insert_node(BST *cur, int64_t key,int64_t value);
bool get_node(BST *root, int64_t key);
void in_order_traversal(BST *root,int thread,int64_t min, int64_t max);
void* handler_insert(void* arg);
void* handler_insert_high_contention(void* arg);
void* handler_get_high_contention(void* arg);
void* handler_traverse(void* arg);
void* handler_get(void* arg);

extern int64_t NO_OF_NODES;
extern BST *root;