#include <iostream>
#include <ucontext.h>

#include "ansys.hpp"

namespace Ansys {

void Ansys::manageExitTaskCtx(void) {
    //static int init = 1;
    //assert(getcontext(&this->exitTaskCtx) == 0);
    //if (init) {
    //    init  = 0;
    //    return;
    //}

    //// The current_task has just exited. Start the next one.
    //bool bootTaskExited = (current_task == boot_task);
    //delete_task(current_task);
    //if (!boot_task_exited) {
    //    ctxsw(find_next_task());
    //} else {
    //    assert(setcontext(&boot_ctx) == 0);
    //}
}

Status Ansys::Boot(void (*bootTask)(void *), void *input) {
    bootTask(input);
    return OK;
}

Status Ansys::CreateTask(void (*fcn)(void *), void *input, int prio) {
    fcn(input);
    return OK;
}

void Ansys::Yield(void) {
}

} // namespace Ansys
