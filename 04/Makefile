CC=g++

all: test

test: test_seria
	./test_seria

test_seria: test.cpp
	$(CC) -o test_seria test.cpp -O2 --std=c++14

clean:
	rm -rf *.o test_seria

