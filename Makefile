ifeq ($(shell uname),Darwin)
$(error "This code doesn't work on Mac OS")
endif

CC=gcc
CFLAGS=-g -Wall -Werror -O0 -Iinc

all: help

help:
	@echo "HELP"
	@echo "----"
	@echo "test: run the tests"
	@echo "clean: remove build artifacts"
	@echo "run-example: run the example application in example/"

bin:
	mkdir $@

bin/%.o: test/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/%.o: src/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/ansys_test: bin/ansys.o bin/ansys_test.o
	$(CC) -o $@ $^ -lpthread

bin/example.o: example/example.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/example: bin/ansys.o bin/example.o
	$(CC) -o $@ $^

.PHONY: test
test: bin/ansys_test
	./bin/ansys_test

.PHONY: run-example
run-example: bin/example
	./$<

.PHONY: clean
clean:
	rm -rf bin
