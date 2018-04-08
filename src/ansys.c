#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ucontext.h>

#if 1
#define note(...) printf(__VA_ARGS__), fflush(stdout);
#else
#define note(...)
#endif

#include "ansys.h"

#define MAX_TASKS 10
#define BOOT_TASK_PRIO 10
#define STACK_SIZE 16384

struct task {
    void (*fcn)(void *);
    void *input;
    int prio;
    int ready;
    int started;

    ucontext_t ctx;
    char stack[STACK_SIZE];
};

static struct task tasks[MAX_TASKS];
static struct task *current_task, *boot_task;
static ucontext_t boot_ctx, exit_task_ctx;

static void init_tasks(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].fcn = NULL;
    }
    current_task = NULL;
}

static void init_task(struct task *t, void (*fcn)(void *), void *input, int prio) {
    t->fcn = fcn;
    t->input = input;
    t->prio = prio;
    t->ready = 1;
    t->started = 0;

    assert(getcontext(&t->ctx) == 0);
    t->ctx.uc_link = &exit_task_ctx;
    sigemptyset(&t->ctx.uc_sigmask);
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = sizeof(t->stack);
    makecontext(&t->ctx, (void(*)(void))fcn, 1, input);
}

static struct task *add_task(void (*fcn)(void *), void *input, int prio) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].fcn == NULL) {
            init_task(&tasks[i], fcn, input, prio);
            return &tasks[i];
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

static void delete_task(struct task *t) {
    t->fcn = NULL;
}

static void ctxsw(struct task *next_task) {
    struct task *previous_task = current_task;
    current_task = next_task;
    assert(swapcontext(&previous_task->ctx, &current_task->ctx) == 0);
}

static void manage_exit_task_ctx(void) {
    static int init = 1;
    assert(getcontext(&exit_task_ctx) == 0);
    if (init) {
        init  = 0;
        return;
    }

    // The current_task has just exited. Start the next one.
    int boot_task_exited = (current_task == boot_task);
    delete_task(current_task);
    if (!boot_task_exited) {
        ctxsw(find_next_task());
    } else {
        assert(setcontext(&boot_ctx) == 0);
    }
}

int ansys_boot(void (*fcn)(void *), void *input) {
    memset(&boot_ctx, 0, sizeof(boot_ctx));
    memset(&exit_task_ctx, 0, sizeof(exit_task_ctx));
    init_tasks();
    manage_exit_task_ctx();

    assert(getcontext(&boot_ctx) == 0);
    if (current_task != NULL) {
        return ERR_BOOTRET;
    }

    current_task = boot_task = add_task(fcn, input, BOOT_TASK_PRIO);
    assert(setcontext(&boot_task->ctx) == 0);

    return ERR_FAILURE;
}

int ansys_create_task(void (*fcn)(void *), int prio) {
    add_task(fcn, NULL, prio);

    ctxsw(find_next_task());

    return ERR_SUCCESS;
}

void ansys_yield(void) {
    current_task->ready = 0;
    struct task *next_task = find_next_task();
    current_task->ready = 1;
    ctxsw(next_task);
}

