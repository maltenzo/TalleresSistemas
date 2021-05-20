CC=g++
CFLAGS=-g -Wall -pedantic -pthread -std=c++0x

all: tests-drivers

tests-drivers: tests-drivers.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

clean:
	rm -rf *.o
	rm -rf tests-drivers
