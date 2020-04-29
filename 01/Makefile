CC=g++

all: test
	./test

test: test_alloc.o Allocator.o
	$(CC) -o test test_alloc.o Allocator.o

test_alloc.o: test_alloc.cpp Allocator.h
	$(CC) -c test_alloc.cpp
	
Allocator.o: Allocator.cpp Allocator.h
	$(CC) -c Allocator.cpp
	
clean:
	rm -rf *.o test
