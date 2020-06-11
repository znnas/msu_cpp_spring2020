CC=g++

all: test

test: su_nu
	./su_nu

su_nu: sum_num_test.cpp
	$(CC) -o su_nu sum_num_test.cpp -pthread

clean:
	rm -rf *.o su_nu