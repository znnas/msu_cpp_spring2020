CC=g++

all: test

test: bigint_test
	./bigint_test

bigint_test: test.cpp bigint.cpp
	$(CC) -o bigint_test test.cpp bigint.cpp 

clean:
	rm -rf *.o bigint_test

