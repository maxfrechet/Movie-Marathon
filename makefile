# makefile executing the compilation process
all: main

main: main.o tree_struct.o list_struct.o
	gcc main.o tree_struct.o list_struct.o -Wall -Wextra -std=c11 -O2 -o main

main.o: main.c
	gcc -c main.c

tree_struct.o: tree_struct.c
	gcc -c tree_struct.c

list_struct.o: list_struct.c
	gcc -c list_struct.c 

clean:
	rm -rf *o main

