#include "../Includes/locks.h"


using namespace std;

int my_sense = 0;


std::atomic<int32_t>  counter_sense(0);
std::atomic<int32_t>  sense(0);


std::atomic<int32_t>  ticket(0);
std::atomic<int32_t> turn(0);	

std::atomic<bool> lock(false) ;
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;





/**************************************Ticket******************************/
void acquire_ticketlock()
{
	//printf("Ticket\n");
	int32_t my_turn = ticket.fetch_add(1, std::memory_order_relaxed);
	//printf("my turn %d\n",my_turn);
	while(turn.load() != my_turn);
}

void release_ticketlock()
{
	turn.fetch_add(1, std::memory_order_relaxed);
}


/**************************************TAS******************************/
void TAS_lock()
{
	//printf("TAS\n");
	while(lock.exchange(true));
}

/**************************************TTAS******************************/

void Test_TAS_lock()
{
	//printf("TTAS\n");
	while(lock);

	while(lock.exchange(true));
}



void unlock()
{
	//printf("Unlock\n");
	__atomic_clear(&lock, __ATOMIC_RELAXED);
}

void pthread_lock()
{
	 pthread_mutex_lock(&mutex_lock);
}

void pthread_unlock()
{
	 pthread_mutex_unlock(&mutex_lock);
}

void pthread_bar()
{
	pthread_barrier_wait(&bar);
}

void sense_bar()
{
	thread_local int my_sense;

	if(my_sense == 0)
		my_sense = 1;
	else
		my_sense = 0;


	int a = counter_sense.fetch_add(1, std::memory_order_relaxed);

	if((a+1) == total_threads)
	{
		counter_sense.store(0);
		sense.store(my_sense);
	}
	else
	{
		while(sense != my_sense);
	}

}
