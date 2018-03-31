#include <stdio.h>
#include <stdlib.h>

#include "test.h"

int __debug = 0;

void __run(void (*test)(void), const char *name, int debug) {
    __debug = debug;
    fprintf(stderr, "RUN: %s\n", name);
    fflush(stderr);
    test();
}

void __skip(void (*test)(void), const char *name, int debug) {
    fprintf(stderr, "SKIP: %s\n", name);
    fflush(stderr);
}

void __equal(int a, const char *a_name, int b, const char *b_name,
        const char *file, int line) {
    if (a != b) {
        __print_status(a, a_name, b, b_name, "FAIL", "==", file, line);
        exit(1);
    } else {
        __print_status(a, a_name, b, b_name, "PASS", "==", file, line);
    }
}

void __print_status(int a, const char *a_name, int b, const char *b_name,
        const char *status, const char *op, const char *file, int line) {
    if (__debug) {
        fprintf(stderr, "  %s: %s (%d) %s %s (%d) @ %s:%d\n",
                (status), a_name, (a), (op), b_name, (b), file, line);
        fflush(stderr);
    }
}
