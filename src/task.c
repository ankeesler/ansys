#include <assert.h>
#include <stddef.h>

#include <ucontext.h>

#include "task.h"

#define MAX_TASKS 10
static struct task tasks[MAX_TASKS];

void init_tasks(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].fcn = NULL;
    }
}

static void init_task(struct task *t, void (*fcn)(void *), void *input, int prio, ucontext_t *exit_ctx) {
    t->fcn = fcn;
    t->input = input;
    t->prio = prio;
    t->ready = 1;
    t->started = 0;

    assert(getcontext(&t->ctx) == 0);
    t->ctx.uc_link = exit_ctx;
    sigemptyset(&t->ctx.uc_sigmask);
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = sizeof(t->stack);
    makecontext(&t->ctx, (void(*)(void))fcn, 1, input);
}

struct task *add_task(void (*fcn)(void *), void *input, int prio, ucontext_t *exit_ctx) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].fcn == NULL) {
            init_task(&tasks[i], fcn, input, prio, exit_ctx);
            return &tasks[i];
        }
    }
    assert("couldn't add task!" == NULL);
}

struct task *find_next_task(void) {
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

void delete_task(struct task *t) {
    t->fcn = NULL;
}
