ifeq ($(shell uname),Darwin)
	CC=clang
else
	CC=gcc
endif

all: help

help:
	@echo "HELP"
	@echo "----"
	@echo "test: run the tests"
	@echo "clean: remove build artifacts"

bin:
	mkdir $@

bin/ansys_test.o: test/ansys_test.c | bin
	$(CC) -o $@ -Iinc -g -Wall -Werror -O0 -c $^

bin/ansys.o: src/ansys.c | bin
	$(CC) -o $@ -Iinc -g -Wall -Werror -Wno-deprecated-declarations -O0 -c $^

bin/ansys_test: bin/ansys.o bin/ansys_test.o
	$(CC) -o $@ $^ -lpthread

bin/example.o: example/example.c | bin
	$(CC) -o $@ -Iinc -g -Wall -Werror -O0 -c $^

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
