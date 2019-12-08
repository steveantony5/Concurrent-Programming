CC = g++
CFLAGS = -Wall -pthread -g -IIncludes/ 
OP = bst

vpath %.c Source/
SRC = bst.c

OBJ :=$(SRC:.c=.o)

mysort : $(OBJ)  #dependencies

	$(CC) $(CFLAGS) -o $(OP) $(OBJ) 

clean: 
	rm -f bst *.o