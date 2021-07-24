all: exec

exec: encoder.c
	gcc -g -m32 -o encoder.o encoder.c

.PHONY: clean
clean:
	rm -rf ./*.o encoder.o
