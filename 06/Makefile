CC=g++

all: test

test: format
	./format_str

format: test.cpp
	$(CC) -o format_str test.cpp

clean:
	rm -rf *.o format_str

