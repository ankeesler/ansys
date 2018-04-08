#include <pthread.h>
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
    p("example: a_task: continuing\n");
    ansys_yield();
    p("example: a_task: end\n");
}

static void boot_task(void *d) {
    p("example: boot_task: start\n");
    ansys_create_task(a_task, 2);
    ansys_create_task(b_task, 1);
    p("example: boot_task: end\n");
}

void *main_routine(void *d) {
    p("example: main_routine: start\n");
    ansys_boot(boot_task, NULL);
    p("example: main_routine: end\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t t;
    pthread_create(&t, NULL, main_routine, NULL);
    pthread_join(t, NULL);
    return 0;
}
