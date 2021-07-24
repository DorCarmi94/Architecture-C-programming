#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: idodo

	
# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
idodo: skeleton.o
	ld -m elf_i386 -g skeleton.o -o idodo

# Depends on the source and header files
skeleton.o: skeleton.s
	nasm -f elf32 -g skeleton.s -o skeleton.o
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o menu
