CC = gcc
CFLAGS = -Wall -Werror -g -IIncludes/
OP = mysort

vpath %.c Source/
SRC = sort_algorithm.c main.c

OBJ :=$(SRC:.c=.o)

mysort : $(OBJ)  #dependencies

	$(CC) $(CFLAGS) -o $(OP) $(OBJ) 

#sort_algorithm.o: sort_algorithm.c sort_algorithm.h 
#		$(CC) $(Flags) $(OBJFLAG) sort_algorithm.c

#main.o: main.c main.h sort_algorithm.o
#		$(CC) $(Flags) $(OBJFLAG) main.c

clean: 
	rm -f mysort *.o