CC = gcc
Flags = -Wall -Werror -g
OBJFLAG = -c


mysort : main.o sort_algorithm.o  #dependencies

	$(CC) -Wall -Werror main.o sort_algorithm.o -o mysort

sort_algorithm.o: sort_algorithm.c sort_algorithm.h 
		$(CC) $(Flags) $(OBJFLAG) sort_algorithm.c

main.o: main.c main.h sort_algorithm.o
		$(CC) $(Flags) $(OBJFLAG) main.c



clean: 
	rm -f mysort *.o