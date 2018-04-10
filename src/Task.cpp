#include <cassert>
#include <csignal>
#include <ucontext.h>

#include "Task.hpp"

namespace Ansys {

Task::Task(void (*fcn)(void *), void *input, int prio, ucontext_t *exitCtx)
    : fcn(fcn), input(input), prio(prio), ready(true) {

    assert(getcontext(&this->ctx) == 0);
    this->ctx.uc_link = exitCtx;
    sigemptyset(&this->ctx.uc_sigmask);
    this->ctx.uc_stack.ss_sp = this->stack;
    this->ctx.uc_stack.ss_size = sizeof(this->stack);
    makecontext(&this->ctx, (void(*)(void))fcn, 1, input);
}

}; // namespace Ansys
