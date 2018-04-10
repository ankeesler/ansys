#include <iostream>

#include "ansys.hpp"

namespace Ansys {

Status Ansys::Boot(void (*bootTask)(void *), void *input) {
    bootTask(input);
    return OK;
}

Status Ansys::CreateTask(void (*fcn)(void *), void *input, int prio) {
    fcn(input);
    return OK;
}

} // namespace Ansys
