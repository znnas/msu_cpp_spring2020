CC=g++

all: test

test: my_threads
	./my_threads

my_threads: threads.cpp
	$(CC) -o my_threads threads.cpp -pthread

clean:
	rm -rf *.o my_threads

