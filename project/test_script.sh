#!/bin/bash

echo "Generating the executable for Handover Mutex"
make clean
make mutex 

echo "Kicking the low contention test case"

echo "Mutex Handover locking"
echo "Low Contention Test Cases checking only insert operation"
echo "Threads = 5, Nodes = 10000"
./bst LOW_CONTENTION 5 10000 1
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst LOW_CONTENTION 10 10000 1
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst LOW_CONTENTION 20 10000 1
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst LOW_CONTENTION 5 50000 1
echo "----"
echo "High Contention Test Cases checking only insert operation"
echo "Threads = 5, Nodes = 10000"
./bst HIGH_CONTENTION 5 10000 1
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst HIGH_CONTENTION 10 10000 1
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst HIGH_CONTENTION 20 10000 1
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst HIGH_CONTENTION 5 50000 1


echo "***************************"

echo "Low Contention Test Cases checking insert, get, range query operations simultaneously"
echo "Threads = 5, Nodes = 10000"
./bst LOW_CONTENTION 5 10000 2
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst LOW_CONTENTION 10 10000 2
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst LOW_CONTENTION 20 10000 2
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst LOW_CONTENTION 5 50000 2
echo "----"
echo "High Contention Test Cases checking only insert operation"
echo "Threads = 5, Nodes = 10000"
./bst HIGH_CONTENTION 5 10000 2
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst HIGH_CONTENTION 10 10000 2
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst HIGH_CONTENTION 20 10000 2
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst HIGH_CONTENTION 5 50000 2

echo "--------------------------------------------------------------------"
echo "Generating the executable for Handover Read Write lock"
make clean
make rwlock 

echo "Kicking the low contention test case"

echo "Read write Handover locking"
echo "Low Contention Test Cases checking only insert operation"
echo "Threads = 5, Nodes = 10000"
./bst LOW_CONTENTION 5 10000 1
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst LOW_CONTENTION 10 10000 1
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst LOW_CONTENTION 20 10000 1
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst LOW_CONTENTION 5 50000 1
echo "----"
echo "High Contention Test Cases checking only insert operation"
echo "Threads = 5, Nodes = 10000"
./bst HIGH_CONTENTION 5 10000 1
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst HIGH_CONTENTION 10 10000 1
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst HIGH_CONTENTION 20 10000 1
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst HIGH_CONTENTION 5 50000 1


echo "***************************"

echo "Low Contention Test Cases checking insert, get, range query operations simultaneously"
echo "Threads = 5, Nodes = 10000"
./bst LOW_CONTENTION 5 10000 2
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst LOW_CONTENTION 10 10000 2
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst LOW_CONTENTION 20 10000 2
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst LOW_CONTENTION 5 50000 2
echo "----"
echo "High Contention Test Cases checking only insert operation"
echo "Threads = 5, Nodes = 10000"
./bst HIGH_CONTENTION 5 10000 2
echo "----"
echo "Threads = 10, Nodes = 10000"
./bst HIGH_CONTENTION 10 10000 2
echo "----"
echo "Threads = 20, Nodes = 10000"
./bst HIGH_CONTENTION 20 10000 2
echo "----"
echo "Threads = 5, Nodes = 50000"
./bst HIGH_CONTENTION 5 50000 2

echo "ALL Test cases are complete"

