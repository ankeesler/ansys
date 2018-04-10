#ifndef __TASK_TABLE_HPP__
#define __TASK_TABLE_HPP__

#include <list>

#include "Task.hpp"

namespace Ansys {

class TaskTable {
    public:
        void AddTask(Task *task);
        Task *FindNextTask(void);
        void DeleteTask(Task *t);

    private:
        std::list<Task*> tasks;
};

}; // namespace Ansys

#endif /* __TASK_TABLE_HPP__ */
