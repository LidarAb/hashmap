.PHONY: all, clean

OBJECTS = libhashmap.a libhashmap_tests.a
CC = gcc
CCFLAGS = -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99

all: $(OBJECTS)

libhashmap.a: hashmap.o vector.o pair.o
	ar rcs $@ $^


libhashmap_tests.a: test_suite.o hashmap.o pair.o vector.o
	ar rcs $@ $^

hashmap.o: hashmap.c hashmap.h vector.h pair.h
	$(CC) $(CCFLAGS) hashmap.c

pair.o: pair.c pair.h
	$(CC) $(CCFLAGS) pair.c

vector.o: vector.c vector.h
	$(CC) $(CCFLAGS) vector.c

test_suite.o: test_suite.c test_suite.h test_pairs.h hash_funcs.h pair.h hashmap.h vector.h
	$(CC) $(CCFLAGS) test_suite.c

clean:
	rm *.o *.a
