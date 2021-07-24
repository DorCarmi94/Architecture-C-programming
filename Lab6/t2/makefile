all: main
main: main.o parser.o 
	gcc -m32 -Wall -g main.o parser.o -o myshell
main.o: task2.c
	gcc -m32 -Wall -c -g  task2.c -o main.o
parser.o: LineParser.c
	gcc -m32 -Wall -c -g  LineParser.c -o parser.o

.PHONY: clean
clean: 
	rm -f *.o main