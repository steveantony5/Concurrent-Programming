CC = g++
CFLAGS = -Wall -pthread -g 

handover: 
	$(CC) $(CFLAGS) -DHANDOVER_LOCKING=1 bst.c -o bst 

rwlock: 
	$(CC) $(CFLAGS) -DHANDOVER_LOCKING=0 bst.c -o bst 


clean: 
	rm -f bst *.o