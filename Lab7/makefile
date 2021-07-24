#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: menu

	
# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
menu: task1.o task4.o
	gcc -g -m32 -Wall -o hexeditplus task1.o
	gcc -g -m32 -fno-pie -Wall -o task4 task4.o

# Depends on the source and header files
task1.o: task1.c
	gcc -m32 -g -Wall -c -o task1.o task1.c

task4.o: task4.c
	gcc -m32 -fno-pie -g -Wall -c -o task4.o task4.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o menu
