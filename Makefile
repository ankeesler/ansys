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

bin/libansys.a: bin/ansys.o bin/task.o
	ar rcs $@ $^

bin/ansys_test: bin/ansys_test.o bin/test.o | bin/libansys.a
	$(CC) -o $@ $^ -lpthread -Lbin -lansys

bin/example: bin/example.o | bin/libansys.a
	$(CC) -o $@ $^ -Lbin -lansys

bin/example-multithread: bin/example-multithread.o | bin/libansys.a
	$(CC) -o $@ $^ -lpthread -Lbin -lansys

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
