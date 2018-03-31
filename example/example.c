#include <stdio.h>

#include "ansys.h"

#define p(...) printf(__VA_ARGS__), fflush(stdout);

static void b_task(void *d) {
    p("example: b_task: start\n");
    ansys_yield();
    p("example: b_task: end\n");
}

static void a_task(void *d) {
    p("example: a_task: start\n");
    ansys_yield();
    p("example: a_task: end\n");
}

static void boot_task(void *d) {
    p("example: boot_task: start\n");
    ansys_create_task(a_task, 1);
    ansys_create_task(b_task, 2);
    p("example: boot_task: end\n");
}

int main(int argc, char *argv[]) {
    p("example: start\n");
    ansys_boot(boot_task, NULL);
    p("example: end\n");
    return 0;
}
