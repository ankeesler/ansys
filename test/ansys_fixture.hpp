#include <queue>
#include <thread>

#include "gtest/gtest.h"
#include "Ansys.hpp"

enum TestEvent {
    BOOT_STARTED,
    BOOT_ENDED,

    TASK_A_STARTED,
    TASK_A_CONTINUED,
    TASK_A_ENDED,

    TASK_B_STARTED,
    TASK_B_ENDED,
};

class AnsysFixture : public ::testing::Test {
    protected:
        void Run(bool runTaskA, bool runTaskB);

        Ansys::Status bootStatus;
        std::queue<TestEvent> events;
};
