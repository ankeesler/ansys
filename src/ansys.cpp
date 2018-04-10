#include "ansys.hpp"

namespace Ansys {

Status Ansys::Boot(void (*bootTask)(void *), void *input) {
    bootTask(input);
    return OK;
}

} // namespace Ansys
