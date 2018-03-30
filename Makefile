all: help

help:
	@echo "HELP"
	@echo "----"
	@echo "test: run the tests"
	@echo "clean: remove build artifacts"

bin:
	mkdir $@

bin/ansys_test.o: test/ansys_test.c | bin
	clang -o $@ -Iinc -g -Wall -Werror -O0 -c $^

bin/ansys.o: src/ansys.c | bin
	clang -o $@ -Iinc -g -Wall -Werror -Wno-deprecated-declarations -O0 -c $^

bin/ansys_test: bin/ansys.o bin/ansys_test.o
	clang -o $@ $^

.PHONY: test
test: bin/ansys_test
	./bin/ansys_test

.PHONY: clean
clean:
	rm -rf bin
