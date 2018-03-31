#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _XOPEN_SOURCE
#include <ucontext.h>

#if 0
#define note(...) printf(__VA_ARGS__), fflush(stdout);
#else
#define note(...)
#endif

#include "ansys.h"

#define MAX_TASKS 10
#define BOOT_TASK_PRIO 10

struct task {
    void (*fcn)(void *);
    void *input;
    int prio;
    int ready;
    int started;

    ucontext_t ctx;
};

static struct task tasks[MAX_TASKS];
static struct task *current_task;

static void init_tasks(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].fcn = NULL;
    }
    current_task = NULL;
}

static void add_task(void (*fcn)(void *), void *input, int prio) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].fcn == NULL) {
            tasks[i].fcn = fcn;
            tasks[i].input = input;
            tasks[i].prio = prio;
            tasks[i].ready = 1;
            tasks[i].started = 0;
            return;
        }
    }
    assert("couldn't add task!" == NULL);
}

static struct task *find_next_task(void) {
    struct task *best = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].fcn != NULL && tasks[i].ready) {
            if (best == NULL || best->prio > tasks[i].prio) {
                best = &tasks[i];
            }
        }
    }

    if (best == NULL) {
        assert("couldn't find next task!" == NULL);
    }

    return best;
}

static void sched(void) {
    static volatile int sw_ctx = 0;
    if (current_task) {
        assert(getcontext(&current_task->ctx) == 0);
        if (sw_ctx) {
            sw_ctx = 0;
            return;
        }
    }
    
    current_task = find_next_task();
    if (current_task->started == 0) {
        current_task->started = 1;
        current_task->fcn(current_task->input);
    } else {
        sw_ctx = 1;
        assert(setcontext(&current_task->ctx) == 0);
    }
}

int ansys_boot(void (*fcn)(void *), void *input) {
    init_tasks();
    add_task(fcn, input, BOOT_TASK_PRIO);
    sched();
    return ERR_FAILURE;
}

int ansys_create_task(void (*fcn)(void *), int prio) {
    add_task(fcn, NULL, prio);
    sched();
    return ERR_SUCCESS;
}

void ansys_yield(void) {
    current_task->ready = 0;
    sched();
}

