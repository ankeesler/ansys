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
#include "task.h"

#define BOOT_TASK_PRIO 10

static struct task *current_task, *boot_task;
static ucontext_t boot_ctx, exit_task_ctx;

static void ctxsw(struct task *next_task) {
    struct task *previous_task = current_task;
    current_task = next_task;
    assert(swapcontext(&previous_task->ctx, &current_task->ctx) == 0);
}

static void manage_exit_task_ctx(void) {
    assert(getcontext(&exit_task_ctx) == 0);
    if (current_task == NULL) {
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
    current_task = boot_task = NULL;
    init_tasks();
    manage_exit_task_ctx();

    assert(getcontext(&boot_ctx) == 0);
    if (current_task != NULL) {
        return ERR_BOOTRET;
    }

    current_task = boot_task = add_task(fcn, input, BOOT_TASK_PRIO, &exit_task_ctx);
    assert(setcontext(&boot_task->ctx) == 0);

    return ERR_FAILURE;
}

int ansys_create_task(void (*fcn)(void *), int prio) {
    add_task(fcn, NULL, prio, &exit_task_ctx);

    ctxsw(find_next_task());

    return ERR_SUCCESS;
}

void ansys_yield(void) {
    current_task->ready = 0;
    struct task *next_task = find_next_task();
    current_task->ready = 1;
    ctxsw(next_task);
}

