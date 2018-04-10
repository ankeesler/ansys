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
    WaitFor(result.done);
    EXPECT_EQ(result.bootStatus, Ansys::OK);
}

TEST_F(AnsysFixture, OneTask) {
}

TEST_F(AnsysFixture, TwoTasks) {
}
