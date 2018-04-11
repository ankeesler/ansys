#include <cstdlib>
#include <iostream>
#include <list>
#include <ucontext.h>

#include "Ansys.hpp"

namespace Ansys {

Ansys::Ansys(void) : currentTask(nullptr), bootTask(nullptr) {
}

Status Ansys::Boot(void (*bootTask)(void *), void *input) {
    this->currentTask = nullptr;
    this->bootTask = nullptr;
    this->manageExitTaskCtx();
    this->manageBootCtx();

    this->currentTask = this->bootTask = this->addTask(bootTask, input, BOOT_TASK_PRIO);
    int err = setcontext(&this->bootCtx);
    if (err != 0) {
        throw "cannot set initial boot context";
    }

    return OK;
}

Status Ansys::CreateTask(void (*fcn)(void *), void *input, int prio) {
    this->addTask(fcn, input, prio);
    this->ctxsw(this->findNextTask());
    return OK;
}

void Ansys::Yield(void) {
    this->currentTask->SetReady(false);
    Task *nextTask = this->findNextTask();
    this->currentTask->SetReady(true);
    this->ctxsw(nextTask);
}

Task *Ansys::addTask(void (*fcn)(void *), void *input, int prio) {
    Task *t = new Task(fcn, input, prio, &this->exitTaskCtx);
    this->tasks.push_back(t);
    return t;
}

Task *Ansys::findNextTask(void) {
    Task *best = nullptr;
    for (std::list<Task*>::iterator it = this->tasks.begin(); it != this->tasks.end(); it++) {
        if ((*it)->Ready() && (best == nullptr || (*it)->Prio() < best->Prio())) {
            best = *it;
        }
    }

    if (best == nullptr) {
        throw "could not find next task!";
    }
    return best;
}

void Ansys::deleteTask(Task *t) {
    this->tasks.remove(t);
    delete t;
}

void Ansys::ctxsw(Task *next) {
    Task *previous = currentTask;
    currentTask = next;

    int err = swapcontext(&previous->Ctx(), &next->Ctx());
    if (err != 0) {
        throw "swapcontext failed in ctxsw()";
    }
}

void Ansys::manageExitTaskCtx(void) {
    int err = getcontext(&this->exitTaskCtx);
    if (err != 0) {
        throw "could not set up exitTaskCtx";
    }
    if (this->currentTask == nullptr) {
        return;
    }

    // The current_task has just exited. Start the next one.
    bool bootTaskExited = (this->currentTask == this->bootTask);
    deleteTask(this->currentTask);
    if (!bootTaskExited) {
        ctxsw(this->findNextTask());
    } else {
        err = setcontext(&this->bootCtx);
        if (err != 0) {
            throw "could not set context to bootCtx";
        }
    }
}

void Ansys::manageBootCtx(void) {
    int err = getcontext(&this->bootCtx);
    if (err != 0 ) {
        throw "could not get bootCtx";
    }
    if (this->currentTask == nullptr) {
        return;
    }
    exit(79);
}

} // namespace Ansys
