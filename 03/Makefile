CC=g++

all: test

test: matrix_test
	./matrix_test

matrix_test: test.cpp
	$(CC) -o matrix_test  test.cpp  -O2 

clean:
	rm -rf *.o matrix_test

