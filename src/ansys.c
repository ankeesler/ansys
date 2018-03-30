#include <assert.h>
#include <stdlib.h>

#define _XOPEN_SOURCE
#include <ucontext.h>

#include "ansys.h"

#define MAX_TASKS 10

struct task {
    void (*fcn)(void *);
    void *input;
};

static struct task tasks[MAX_TASKS];

static void init_tasks(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].fcn = NULL;
    }
}

static void add_task(void (*fcn)(void *), void *input) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].fcn == NULL) {
            tasks[i].fcn = fcn;
            tasks[i].input = input;
            return;
        }
    }
    assert("couldn't add task!" == NULL);
}

static struct task *find_next_task(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].fcn != NULL) {
            return &tasks[i];
        }
    }
    assert("couldn't find next task!" == NULL);
}

static void sched(void) {
    struct task *t = find_next_task();
    t->fcn(t->input);
}

int ansys_boot(void (*fcn)(void *), void *input) {
    init_tasks();
    add_task(fcn, input);
    sched();
    return ERR_FAILURE;
}

int ansys_create_task(void (*fcn)(void *)) {
    fcn(NULL);
    return ERR_SUCCESS;
}

void ansys_yield(void) {
    //ucontext_t ctx;
    //if (setcontext(&ctx) != 0) {
    //    assert("couldn't set context!" == NULL);
    //}
}

