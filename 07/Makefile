CC=g++

all: test

test: vector
	./vector

vector: test.cpp
	$(CC) -o vector test.cpp -O2 --std=c++14

clean:
	rm -rf *.o vector

