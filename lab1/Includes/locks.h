#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

using namespace std;

class MCS_node
{
public:
	std::atomic<MCS_node*>  next;
	std::atomic<bool>  got_it;
};


extern std::atomic<MCS_node*> tail;


class MCS
{
public:
	void mcs_acquire(MCS_node *node)
	{
		MCS_node *old_tail = tail.load();
		node->next.store(NULL, memory_order_relaxed);
		while (!tail.compare_exchange_strong(old_tail, node)) 
		{
			old_tail = tail.load();
		}
		if (old_tail != NULL) 
		{
			node->got_it.store(true, memory_order_relaxed);
			old_tail->next.store(node);
			while (node->got_it.load());
		}
	}



	void mcs_release(MCS_node *node)
	{
		MCS_node* temp_node = node;
		if (tail.compare_exchange_strong(temp_node, NULL)) 
		{

		} 
		else 
		{
			while (node->next.load() == NULL);
			node->next.load()->got_it.store(false);
		}
	}

};


extern pthread_barrier_t bar;
extern int total_threads;

void acquire_ticketlock();

void release_ticketlock();

void TAS_lock();
void Test_TAS_lock();

void unlock();

void mcs_acquire();

void mcs_release();

void pthread_lock();

void pthread_unlock();

void pthread_bar();

void sense_bar();