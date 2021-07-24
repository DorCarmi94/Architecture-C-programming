all: main
main: main.o 
	gcc -m32 -Wall -g main.o -o myshell
main.o: task3.c
	gcc -m32 -Wall -c -g  task3.c -o main.o
.PHONY: clean
clean: 
	rm -f *.o main