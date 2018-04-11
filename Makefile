ifeq ($(shell uname),Darwin)
$(error "This code doesn't work on Mac OS")
endif

CC=gcc
CXX=g++
CFLAGS=-g -Wall -Werror -O0 -Iinc
CXXFLAGS=-g -Wall -Werror -O0 -Iinc -std=c++11

all: help

help:
	@echo "HELP"
	@echo "----"
	@echo "test: run the tests (with the C implementation)"
	@echo "testpp: run the tests (with the C++ implementation)"
	@echo "clean: remove build artifacts"
	@echo "run-example: run the example application in example/"
	@echo "run-example-multithread: run the multithreaded example application in example/"

bin:
	mkdir $@

test/googletest/googletest/make/gtest_main.a:
	CXX=$(CXX) make -C test/googletest/googletest/make gtest_main.a

bin/gtest_main.a: test/googletest/googletest/make/gtest_main.a | bin
	mv $< $@

bin/%.o: test/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/ansys_fixture-cpp.o: test/ansys_fixture.cpp test/ansys_fixture.hpp | bin
	$(CXX) -o $@ $(CXXFLAGS) -Itest/googletest/googletest/include/ -c $<

bin/ansys_test-cpp.o: test/ansys_test.cpp test/ansys_fixture.hpp | bin
	$(CXX) -o $@ $(CXXFLAGS) -Itest/googletest/googletest/include/ -c $<

bin/%.o: src/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/%-cpp.o: src/%.cpp | bin
	$(CXX) -o $@ $(CXXFLAGS) -c $^

bin/libansys.a: bin/Ansys-cpp.o bin/Task-cpp.o | bin
	ar rcs $@ $^

bin/%.o: example/%.c | bin
	$(CC) -o $@ $(CFLAGS) -c $^

bin/ansys_test: bin/ansys.o bin/ansys_test.o bin/test.o
	$(CC) -o $@ $^ -lpthread

bin/ansys_cpp_test: bin/libansys.a bin/gtest_main.a bin/ansys_fixture-cpp.o bin/ansys_test-cpp.o
	$(CXX) -o $@ $^ -lpthread

bin/example: bin/ansys.o bin/example.o
	$(CC) -o $@ $^

bin/example-multithread: bin/ansys.o bin/example-multithread.o
	$(CC) -o $@ $^ -lpthread

.PHONY: test
test: bin/ansys_test
	./bin/ansys_test

.PHONY: testpp
testpp: bin/ansys_cpp_test
	./bin/ansys_cpp_test

.PHONY: run-example
run-example: bin/example
	./$<

.PHONY: run-example-multithread
run-example-multithread: bin/example-multithread
	./$<

.PHONY: clean
clean:
	rm -rf bin
