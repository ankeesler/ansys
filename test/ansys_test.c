#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "ansys.h"

#define TIMEOUT_S 2

#define run(test, debug) \
    do { \
        __debug = (debug); \
        fprintf(stderr, "TEST: %s\n", #test); \
        (test)(); \
    } while(0);
#define skip(test, debug) (void)(test)

#define __print_status(a, b, status, op) \
    do { \
        if (__debug) { \
            fprintf(stderr, "  %s: %s (%d) %s %s (%d) @ %s:%d\n", (status), #a, (a), (op), #b, (b), __FILE__, __LINE__); \
            fflush(stderr); \
        } \
    } while (0);

#define __print_success(a, b) __print_status((a), (b), "PASS", "==")

#define __print_failure(a, b) __print_status((a), (b), "FAIL", "!=")

#define equal(a, b) \
    do { \
        if ((a) != (b)) { \
            __print_failure((a), (b)); \
            exit(1); \
        } else { \
            __print_success((a), (b)); \
        } \
    } while (0);

#define equal_eventually(a, b) \
    do { \
        time_t now = time(NULL); \
        do { \
            sleep(1); \
            if ((time(NULL) - now) > TIMEOUT_S) { \
                __print_failure((a), (b)); \
                exit(1); \
            } \
        } while((a) != (b)); \
        __print_success((a), (b)); \
    } while (0);

static int __debug = 0;

static int a_started = 0;
static int a_ended = 0;
static void a_task(void *d) {
    a_started = 1;

    while (1) { ansys_yield(); }
    a_ended = 1;
}

static int b_started = 0;
static int b_ended = 0;
static void b_task(void *d) {
    b_started = 1;

    while (1) { ansys_yield(); }
    b_ended = 1;
}

struct boot_input {
    int magic;
    int tasks;

    volatile int stop;
};
static int boot_started = 0;
static int boot_ended = 0;
static void boot_task(void *input) {
    struct boot_input *bi = (struct boot_input *)input;
    equal(bi->magic, 0xAC0000AC);

    boot_started = 1;

    if (bi->tasks == 1) {
        equal(ansys_create_task(a_task, 1), ERR_SUCCESS);
    } else if (bi->tasks == 2) {
        equal(ansys_create_task(a_task, 1), ERR_SUCCESS);
        equal(ansys_create_task(b_task, 2), ERR_SUCCESS);
    }

    while (1) {
        sleep(1);
        if (bi->stop) {
            pthread_exit(NULL);
        }
    }
    boot_ended = 1;
}

static pthread_t sys_thread;
static void *sys_routine(void *data) {
    struct boot_input *bi = (struct boot_input *)data;
    ansys_boot(boot_task, bi); // SHOULD NOT RETURN
    equal(0, 1);
    return NULL;
}

static void test_basic_boot(void) {
    struct boot_input input;
    input.magic = 0xAC0000AC;
    input.tasks = 0;
    input.stop = 0;

    int err = pthread_create(&sys_thread, NULL, sys_routine, (void *)&input);
    equal(err, 0);

    equal_eventually(boot_started, 1);
    equal_eventually(boot_ended, 0);

    input.stop = 1;
    err = pthread_join(sys_thread, NULL);
    equal(err, 0);
}

static void test_create_one_task(void) {
    struct boot_input input;
    input.magic = 0xAC0000AC;
    input.tasks = 1;
    input.stop = 0;

    int err = pthread_create(&sys_thread, NULL, sys_routine, (void *)&input);
    equal(err, 0);

    equal_eventually(boot_started, 1);
    equal_eventually(boot_ended, 0);

    equal_eventually(a_started, 1);
    equal_eventually(a_ended, 0);

    input.stop = 1;
    err = pthread_join(sys_thread, NULL);
    equal(err, 0);
}

static void test_create_two_tasks(void) {
    struct boot_input input;
    input.magic = 0xAC0000AC;
    input.tasks = 2;

    int err = pthread_create(&sys_thread, NULL, sys_routine, (void *)&input);
    equal(err, 0);

    equal_eventually(boot_started, 1);
    equal_eventually(boot_ended, 0);

    equal_eventually(a_started, 1);
    equal_eventually(a_ended, 0);

    equal_eventually(b_started, 1);
    equal_eventually(b_ended, 0);

    input.stop = 1;
    err = pthread_join(sys_thread, NULL);
    equal(err, 0);
}

int main(int argc, char *argv[]) {
    run(test_basic_boot, 0);
    run(test_create_one_task, 0);
    run(test_create_two_tasks, 0);
    return 0;
}
