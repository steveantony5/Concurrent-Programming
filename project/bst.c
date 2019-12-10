//Includes
#include "bst.h"

//Globals
int64_t NO_OF_NODES;



std::vector<struct pair_t> vec[MAX_THREADS]; 


BST *root = NULL;


#if MUTEX_LOCKING

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#else
pthread_rwlock_t lock ;

#endif



//Functions with mutex handover locking
#if MUTEX_LOCKING
/* Insert function of BST
 This function inserts key value pairs into the BST
 keys are unique and if a key is inserted which is already present in bst, it just replaces the value

 Parameters:
 cur - node of the BST
 key - key to be inserted
 value - value for the corresponding key
*/
bool insert_node(BST *cur, int64_t key,int64_t value)
{
	if(cur == NULL)
	{
		pthread_mutex_lock(&lock);

		if(root == NULL)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
			if(new_child != NULL)
			{
				pthread_mutex_init(&(new_child->node_lock),NULL);
				new_child->key = key;
				new_child->value = value;
				new_child->LC = NULL;
				new_child->RC = NULL;
				root = new_child;
				pthread_mutex_unlock(&lock);
				return true;
			}
		}
		else
		{
			pthread_mutex_lock(&(root->node_lock));
			cur = root;
			pthread_mutex_unlock(&lock);
		}

	}

	if(key < cur->key)
	{
		if(cur->LC == NULL)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
			if(new_child != NULL)
			{
				pthread_mutex_init(&(new_child->node_lock),NULL);
				new_child->key = key;
				new_child->value = value;
				new_child->LC = NULL;
				new_child->RC = NULL;
				cur->LC = new_child;
				pthread_mutex_unlock(&(cur->node_lock));
				return true;
			}
		}
		else
		{
			pthread_mutex_lock(&(cur->LC->node_lock));
			pthread_mutex_unlock(&(cur->node_lock));
			insert_node(cur->LC, key,value);
		}
	}
	else if(key > cur->key)
	{
		if(cur->RC == NULL)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
			if(new_child != NULL)
			{
				pthread_mutex_init(&(new_child->node_lock),NULL);
				new_child->key = key;
				new_child->value = value;
				new_child->LC = NULL;
				new_child->RC = NULL;
				cur->RC = new_child;
				pthread_mutex_unlock(&(cur->node_lock));
				return true;
			}
		}
		else
		{
			pthread_mutex_lock(&(cur->RC->node_lock));
			pthread_mutex_unlock(&(cur->node_lock));
			insert_node(cur->RC, key,value);
		}
	}

	else
	{
		cur->value = value;
		pthread_mutex_unlock(&(cur->node_lock));
		return true;
	}
	return true;


}


/* Get function of BST
 This function checks if a key exists in the BST

 Parameters:
 root - node of the BST
 key  - key to be checked if it exists

*/
bool get_node(BST *root, int64_t key)
{
	
	pthread_mutex_lock(&(root->node_lock));
    while (root != NULL) 
    { 
        if (key > root->key) 
        {
        	
        	pthread_mutex_unlock(&(root->node_lock));
            root = root->RC; 

            if(root != NULL)
            {
            	
           	    pthread_mutex_lock(&(root->node_lock));
            }
        }
  
        else if (key < root->key) 
        {
        	
        	pthread_mutex_unlock(&(root->node_lock));
            root = root->LC; 

            if(root != NULL)
            {
            	
            	pthread_mutex_lock(&(root->node_lock));
            }
        }
        else
        {
        	
        	pthread_mutex_unlock(&(root->node_lock));
            return true; 
        }
    } 
    return false; 

}

/* Range query of BST
 This function copies the key value pairs for which the key is 
 between the min and max value of the range query

 Parameters:
 root - node
 thread - thread number to choose the bucket in the vector 
 min - minimum of the requested range
 max - maximum of the requested range

*/
void in_order_traversal(BST *root,int thread,int64_t min, int64_t max)
{

	if(root == NULL)
	{

		return;
	}
	
	pthread_mutex_unlock(&(root->node_lock));

	if(root->LC != NULL)
	{
		pthread_mutex_lock(&((root->LC)->node_lock));
	}

	in_order_traversal(root->LC,thread, min, max);

	if((root->key >= min) && (root->key <= max))
	{
		struct pair_t pair_value;
		pair_value.value = root->value;
		pair_value.key   = root->key;

		vec[thread].push_back(pair_value);
	}

	if(root->RC != NULL)
	{	
		pthread_mutex_lock(&((root->RC)->node_lock));
	}

	
	in_order_traversal(root->RC,thread, min, max);

}

//Functions with read write locking
#else


/* Insert function of BST
 This function inserts key value pairs into the BST
 keys are unique and if a key is inserted which is already present in bst, it just replaces the value

 Parameters:
 cur - node of the BST
 key - key to be inserted
 value - value for the corresponding key
*/
bool insert_node(BST *cur, int64_t key,int64_t value)
{
	if(cur == NULL)
	{
		pthread_rwlock_wrlock(&lock);

		if(root == NULL)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
			if(new_child != NULL)
			{
				pthread_rwlock_init(&(new_child->node_lock),NULL);
				new_child->key = key;
				new_child->value = value;
				new_child->LC = NULL;
				new_child->RC = NULL;
				root = new_child;
				pthread_rwlock_unlock(&lock);
				return true;
			}
		}
		else
		{
			pthread_rwlock_wrlock(&(root->node_lock));
			cur = root;
			pthread_rwlock_unlock(&lock);
		}

	}

	if(key < cur->key)
	{
		if(cur->LC == NULL)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
			if(new_child != NULL)
			{
				pthread_rwlock_init(&(new_child->node_lock),NULL);
				new_child->key = key;
				new_child->value = value;
				new_child->LC = NULL;
				new_child->RC = NULL;
				cur->LC = new_child;
				pthread_rwlock_unlock(&(cur->node_lock));
				return true;
			}
		}
		else
		{
			pthread_rwlock_wrlock(&(cur->LC->node_lock));
			pthread_rwlock_unlock(&(cur->node_lock));
			insert_node(cur->LC, key,value);
		}
	}
	else if(key > cur->key)
	{
		if(cur->RC == NULL)
		{
			BST *new_child = (BST *)malloc(sizeof(BST));
			if(new_child != NULL)
			{
				pthread_rwlock_init(&(new_child->node_lock),NULL);
				new_child->key = key;
				new_child->value = value;
				new_child->LC = NULL;
				new_child->RC = NULL;
				cur->RC = new_child;
				pthread_rwlock_unlock(&(cur->node_lock));
				return true;
			}
		}
		else
		{
			pthread_rwlock_wrlock(&(cur->RC->node_lock));
			pthread_rwlock_unlock(&(cur->node_lock));
			insert_node(cur->RC, key,value);
		}
	}

	else
	{
		cur->value = value;
		pthread_rwlock_unlock(&(cur->node_lock));
		return true;
	}
	return true;

}

/* Get function of BST
 This function checks if a key exists in the BST

 Parameters:
 root - node of the BST
 key  - key to be checked if it exists

*/

bool get_node(BST *root, int64_t key)
{
	
	pthread_rwlock_rdlock(&(root->node_lock));
    while (root != NULL) 
    { 
        if (key > root->key) 
        {
        	
        	pthread_rwlock_unlock(&(root->node_lock));
            root = root->RC; 

            if(root != NULL)
            {
            	
           	    pthread_rwlock_rdlock(&(root->node_lock));
            }
        }
  
        else if (key < root->key) 
        {
        	
        	pthread_rwlock_unlock(&(root->node_lock));
            root = root->LC; 

            if(root != NULL)
            {
            	
            	pthread_rwlock_rdlock(&(root->node_lock));
            }
        }
        else
        {
        	
        	pthread_rwlock_unlock(&(root->node_lock));
            return true; 
        }
    } 
    return false; 

}

/* Range query of BST
 This function copies the key value pairs for which the key is 
 between the min and max value of the range query

 Parameters:
 root - node
 thread - thread number to choose the bucket in the vector 
 min - minimum of the requested range
 max - maximum of the requested range

*/
void in_order_traversal(BST *root,int thread, int64_t min, int64_t max)
{

	if(root == NULL)
	{

		return;
	}
	
	pthread_rwlock_unlock(&(root->node_lock));

	if(root->LC != NULL)
	{
		pthread_rwlock_rdlock(&((root->LC)->node_lock));
	}

	in_order_traversal(root->LC,thread , min, max);
	
	if((root->key >= min) && (root->key <= max))
	{
		struct pair_t pair_value;
		pair_value.value = root->value;
		pair_value.key   = root->key;

		vec[thread].push_back(pair_value);
	}

	if(root->RC != NULL)
	{	
		pthread_rwlock_rdlock(&((root->RC)->node_lock));
	}

	
	in_order_traversal(root->RC,thread,  min, max);

}


#endif

/* Thread handlers */

//thread handler for insert thread for low contention scenerio
void* handler_insert(void* arg)
{

	//printf("insert thread begin for %ld \n",pthread_self());
  int64_t key = (int64_t)arg;

   for(int64_t i=1,j = key; i< NO_OF_NODES; i++, j++)
	{
		insert_node(NULL,j,rand());
	}
	//printf("insert completed for %ld \n",pthread_self());

	return NULL;
}

//thread handler for insert thread for high contention scenerio
void* handler_insert_high_contention(void* arg)
{

	//printf("insert thread begin for %ld \n",pthread_self());

   for(int64_t i=1; i< NO_OF_NODES; i++)
	{
		insert_node(NULL,(i % 10),rand());
	}
	//printf("insert completed for %ld \n",pthread_self());

	return NULL;
}

//thread handler for get thread for high contention scenerio
void* handler_get_high_contention(void* arg)
{
	//printf("get thread begin for %ld \n",pthread_self());

    for(int64_t i=1; i< NO_OF_NODES; i++)
	{
		get_node(root,(i % 10));
	}

	
	//printf("get completed for %ld \n",pthread_self());

	return NULL;

}

//thread handler for traverse thread for low/high contention scenerio
void* handler_traverse(void* arg)
{
	//printf("Traverse thread begin for %ld \n",pthread_self());
   int64_t thread = (int64_t) arg;

   if(root == NULL)
   {
   		printf("BST doesn't exists\n");
   		return NULL;
   }

   #if MUTEX_LOCKING
   pthread_mutex_lock(&(root->node_lock));

   #else
   pthread_rwlock_rdlock(&(root->node_lock));
   #endif

   in_order_traversal(root,thread,INT_MIN, INT_MAX);
   //printf("Traverse completed for %ld \n",pthread_self());

   return NULL;

}

//thread handler for get thread for low contention scenerio
void* handler_get(void* arg)
{
	//printf("get thread begin for %ld \n",pthread_self());
  int64_t key = (int64_t)arg;

    for(int64_t i=1,j = key; i< 1000; i++, j++)
	{
		get_node(root,j);
	}

	
	//printf("get completed for %ld \n",pthread_self());

	return NULL;

}


