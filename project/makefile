CC = g++
CFLAGS = -Wall -pthread -g 

mutex:
	$(CC) $(CFLAGS) -DMUTEX_LOCKING=1 main.c bst.c -o bst 

rwlock: 
	$(CC) $(CFLAGS) -DMUTEX_LOCKING=0 main.c bst.c -o bst 



clean: 
	rm -f bst *.o