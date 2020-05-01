CC=g++

all: test

test: parser_test
	./parser_test

parser_test: test_parser.cpp my_parser.cpp
	$(CC) -o parser_test my_parser.cpp test_parser.cpp -O2

clean:
	rm -rf *.o parser_test
