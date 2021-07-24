#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: menu

	
# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
menu: menu_map.o
	gcc -g -m32 -Wall -o menu menu_map.o

# Depends on the source and header files
menu_map.o: menu_map.c
	gcc -m32 -g -Wall -c -o menu_map.o menu_map.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o menu
