
#include <ucontext.h>

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

void init_tasks(void);
struct task *add_task(void (*fcn)(void *), void *input, int prio, ucontext_t *exit_ctx);
struct task *find_next_task(void);
void delete_task(struct task *t);
