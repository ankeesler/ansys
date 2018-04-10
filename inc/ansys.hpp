#ifndef __ANSYS_HPP__
#define __ANSYS_HPP__

namespace Ansys {

enum Status {
    UNKNOWN = -1,
    OK = 0,
};

class Ansys {
    public:
        Status Boot(void (*boot_task)(void *), void *input);
};

} // namespace Ansys

#endif /* #ifdef __ANSYS_HPP__ */
