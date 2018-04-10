#include <ctime>
#include <iostream>
#include <queue>

#include "ansys.hpp"
#include "ansys_fixture.hpp"

TEST_F(AnsysFixture, BasicBoot) {
    Run(false, false);
    EXPECT_EQ(bootStatus, Ansys::OK);
    std::queue<TestEvent> expected;
    expected.push(TestEvent::BOOT_STARTED);
    expected.push(TestEvent::BOOT_ENDED);
    EXPECT_EQ(expected, events);
}

TEST_F(AnsysFixture, OneTask) {
    Run(true, false);
    EXPECT_EQ(bootStatus, Ansys::OK);
    std::queue<TestEvent> expected;
    expected.push(TestEvent::BOOT_STARTED);
    expected.push(TestEvent::TASK_A_STARTED);
    expected.push(TestEvent::TASK_A_ENDED);
    expected.push(TestEvent::BOOT_ENDED);
    EXPECT_EQ(expected, events);
}

TEST_F(AnsysFixture, TwoTasks) {
    Run(true, true);
    EXPECT_EQ(bootStatus, Ansys::OK);
    std::queue<TestEvent> expected;
    expected.push(TestEvent::BOOT_STARTED);
    expected.push(TestEvent::TASK_A_STARTED);
    expected.push(TestEvent::TASK_B_STARTED);
    expected.push(TestEvent::TASK_B_ENDED);
    expected.push(TestEvent::TASK_A_ENDED);
    expected.push(TestEvent::BOOT_ENDED);
    EXPECT_EQ(expected, events);
}
