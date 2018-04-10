#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <ucontext.h>

namespace Ansys {

#define STACK_SIZE 1024

class Task {
    public:
        Task(void (*fcn)(void *), void *input, int prio);

    private:
        void (*fcn)(void *);
        void *input;
        int prio;
        bool ready;

        ucontext_t ctx;
        char stack[STACK_SIZE];
};

}; // namespace Ansys

#endif /* __TASK_HPP__ */
