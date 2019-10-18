CC = g++
CFLAGS = -Wall -Werror -g -pthread 
OP = mysort

all: Source/main.cpp Includes/main.h locks.o sort_algorithm_fj.o sort_algorithm_bucket.o
		$(CC) $(CFLAGS)  Source/main.cpp locks.o sort_algorithm_fj.o sort_algorithm_bucket.o -o mysort

counter: Source/counter.cpp locks.o
		$(CC) $(CFLAGS)  Source/counter.cpp locks.o -o counter

locks.o: Source/locks.cpp Includes/locks.h
		$(CC) $(CFLAGS) -c Source/locks.cpp 

sort_algorithm_fj.o: Source/sort_algorithm_fj.cpp Includes/sort_algorithm_fj.h 
		$(CC) $(CFLAGS) -c Source/sort_algorithm_fj.cpp 

sort_algorithm_bucket.o: Source/sort_algorithm_bucket.cpp Includes/sort_algorithm_bucket.h
		$(CC) $(CFLAGS) -c Source/sort_algorithm_bucket.cpp 



clean: 
	rm -f mysort counter *.o