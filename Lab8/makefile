#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: menu

	
# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
menu: task3.o task2.o task1.o
	gcc -g -m32 -Wall -o myELF task3.o
	gcc -g -m32 -Wall -o myELF2 task2.o
	gcc -g -m32 -Wall -o myELF1 task1.o

# Depends on the source and header files

task3.o: task3.c
	gcc -m32 -g -Wall -c -o task3.o task3.c



task2.o: task2.c
	gcc -m32 -g -Wall -c -o task2.o task2.c


task1.o: task1.c
	gcc -m32 -g -Wall -c -o task1.o task1.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o menu
