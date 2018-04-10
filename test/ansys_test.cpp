#include <ctime>
#include <iostream>

#include "ansys.hpp"
#include "ansys_fixture.hpp"

#define TIMEOUT_S 3

#define WaitFor(a) \
    do { \
        time_t now = time(NULL); \
        do { \
            sleep(1); \
            if ((time(NULL) - now) > TIMEOUT_S) { \
                ASSERT_EQ(123, 456); \
            } \
        } while(!(a)); \
    } while (0);

TEST_F(AnsysFixture, BasicBoot) {
    Start(false, false);
    WaitFor(data.output.done);
    EXPECT_EQ(data.output.bootStatus, Ansys::OK);
    EXPECT_TRUE(data.output.bootStarted);
    EXPECT_FALSE(data.output.taskAStarted);
    EXPECT_FALSE(data.output.taskBStarted);
    EXPECT_FALSE(data.output.taskBFinished);
    EXPECT_FALSE(data.output.taskAFinished);
    EXPECT_TRUE(data.output.bootFinished);
}

TEST_F(AnsysFixture, OneTask) {
    Start(true, false);
    WaitFor(data.output.done);
    EXPECT_EQ(data.output.bootStatus, Ansys::OK);
    EXPECT_TRUE(data.output.bootStarted);
    EXPECT_TRUE(data.output.taskAStarted);
    EXPECT_FALSE(data.output.taskBStarted);
    EXPECT_FALSE(data.output.taskBFinished);
    EXPECT_TRUE(data.output.taskAFinished);
    EXPECT_TRUE(data.output.bootFinished);
}

TEST_F(AnsysFixture, TwoTasks) {
    Start(true, true);
    WaitFor(data.output.done);
    EXPECT_EQ(data.output.bootStatus, Ansys::OK);
    EXPECT_TRUE(data.output.bootStarted);
    EXPECT_TRUE(data.output.taskAStarted);
    EXPECT_TRUE(data.output.taskBStarted);
    EXPECT_TRUE(data.output.taskBFinished);
    EXPECT_TRUE(data.output.taskAFinished);
    EXPECT_TRUE(data.output.bootFinished);
}
