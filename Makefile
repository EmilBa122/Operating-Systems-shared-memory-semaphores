# Makefile

# *****************************************************

CC = gcc

# ****************************************************

all: processes p2

processes: processes.o structure.o
	$(CC) -o processes processes.o structure.o

p2: p2.o structure.o
	$(CC) -o p2 p2.o structure.o

processes.o: processes.c structure.h
	$(CC) -c processes.c

p2.o: p2.c structure.h
	$(CC) -c p2.c

structure.o: structure.c structure.h
	$(CC) -c structure.c 

clean:
	rm -rf *.o
	rm -rf processes
	rm -rf p2
