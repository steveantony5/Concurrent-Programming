#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <atomic>

using namespace std;


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

struct node* insert_BST(int key, struct node *root)
{
	//printf("key %d\n",key);
	if(root == NULL)
	{
		BST *node_n	 = (BST *)malloc(sizeof(BST));
		if(node_n != NULL)
		{
			node_n->key = key;
			node_n->right_node = NULL;
			node_n->left_node = NULL;
			return node_n;
		}
		else
		{
			return NULL;
		}
	}


	else
	{
		//printf("key %d Root->key %d\n",key,(root->key));
		if(key > (root->key))
		{
			//printf("right\n");
			(root->right_node) = 	insert_BST(key,(root->right_node));

		}
		if(key < (root->key))
		{
			//printf("left\n");
			(root->left_node) =	insert_BST(key,(root->left_node));
		}
	}
	return root;
}
