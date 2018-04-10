#include "TaskTable.hpp"
#include "Task.hpp"

namespace Ansys {

void TaskTable::AddTask(Task *task) {
    this->tasks.push_back(task);
}

Task *TaskTable::FindNextTask(void) {
    throw "implement me!";
    return nullptr;
}

void TaskTable::DeleteTask(Task *t) {
    throw "implement me!";
}

}; // namespace Ansys
