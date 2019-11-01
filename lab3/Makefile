CC = gcc
CFLAGS = -Wall -Werror -g -fopenmp -IIncludes/ 
OP = mysort

vpath %.c Source/
SRC = sort_algorithm.c main.c

OBJ :=$(SRC:.c=.o)

mysort : $(OBJ)  #dependencies

	$(CC) $(CFLAGS) -o $(OP) $(OBJ) 

clean: 
	rm -f mysort *.o