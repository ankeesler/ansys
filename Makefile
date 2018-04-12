ifeq ($(shell uname),Darwin)
$(error "This code doesn't work on Mac OS")
endif

CC=gcc
CFLAGS=-g -Wall -Werror -O0 -Iinc

all: help

help:
	@echo "HELP"
	@echo "----"
	@echo "test: run the tests (with the C implementation)"
	@echo "clean: remove build artifacts"
	@echo "run-example: run the example application in example/"
	@echo "run-example-multithread: run the multithreaded example application in example/"

bin:
	mkdir $@

bin/%.o: test/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/%.o: src/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/%.o: example/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/ansys_test: bin/ansys.o bin/ansys_test.o bin/test.o
	$(CC) -o $@ $^ -lpthread

bin/example: bin/ansys.o bin/example.o
	$(CC) -o $@ $^

bin/example-multithread: bin/ansys.o bin/example-multithread.o
	$(CC) -o $@ $^ -lpthread

.PHONY: test
test: bin/ansys_test
	./bin/ansys_test

.PHONY: run-example
run-example: bin/example
	./$<

.PHONY: run-example-multithread
run-example-multithread: bin/example-multithread
	./$<

.PHONY: clean
clean:
	rm -rf bin
