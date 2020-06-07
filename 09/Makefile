CC=g++

all: test

test: sort_thread
	./sort_thread

sort_thread: sort_thread.cpp
	$(CC) -o sort_thread sort_thread.cpp uint64.cpp -pthread

clean:
	rm -rf *.o sort_thread

