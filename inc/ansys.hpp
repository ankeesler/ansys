#ifndef __ANSYS_HPP__
#define __ANSYS_HPP__

#include <ucontext.h>

namespace Ansys {

enum Status {
    UNKNOWN = -1,
    OK = 0,
};

class Ansys {
    public:
        Status Boot(void (*bootTask)(void *input), void *input);
        Status CreateTask(void (*fcn)(void *input), void *input, int prio);
        void Yield(void);

    private:
        void manageExitTaskCtx(void);

        ucontext_t bootCtx, exitTaskCtx;
};

} // namespace Ansys

#endif /* #ifdef __ANSYS_HPP__ */
