#ifndef __ANSYS_HPP__
#define __ANSYS_HPP__

#include <list>

#include <ucontext.h>

#include "../src/Task.hpp"

namespace Ansys {

enum Status {
    UNKNOWN = -1,
    OK = 0,
    BOOTRET,
};

const int BOOT_TASK_PRIO = 10;

class Ansys {
    public:
        Ansys(void);
        Status Boot(void (*bootTask)(void *input), void *input);
        Status CreateTask(void (*fcn)(void *input), void *input, int prio);
        void Yield(void);

    private:
        Task *addTask(void (*fcn)(void *), void *input, int prio);
        Task *findNextTask(void);
        void deleteTask(Task *t);

        void ctxsw(Task *nextTask);
        void manageExitTaskCtx(void);
        void manageBootCtx(void);

        ucontext_t bootCtx, exitTaskCtx;
        std::list<Task*> tasks;
        Task *currentTask, *bootTask;
};

} // namespace Ansys

#endif /* #ifdef __ANSYS_HPP__ */
