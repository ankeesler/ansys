#ifndef __ANSYS_HPP__
#define __ANSYS_HPP__

namespace Ansys {

enum Status {
    UNKNOWN = -1,
    OK = 0,
};

class Ansys {
    public:
        Status Boot(void (*bootTask)(void *input), void *input);
        Status CreateTask(void (*fcn)(void *input), void *input, int prio);
};

} // namespace Ansys

#endif /* #ifdef __ANSYS_HPP__ */
