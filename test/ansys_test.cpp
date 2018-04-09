#include <thread>

#include "gtest/gtest.h"
#include "ansys.hpp"

class AnsysFixture : public ::testing::Test {
    protected:
        Ansys::Ansys sys;
        Ansys::Status bootStatus;

        void SetUp(void) {
            this->sys_thread = new std::thread(sys_routine, std::ref(this->sys), &this->bootStatus);
        }

        virtual void TearDown(void) {
            this->sys_thread->join();
            delete this->sys_thread;
        }

    private:
        std::thread *sys_thread;

        static void sys_routine(Ansys::Ansys& sys, Ansys::Status *bootStatus) {
            *bootStatus = sys.Boot(nullptr, nullptr);
        }
};

TEST_F(AnsysFixture, BasicBoot) {
    EXPECT_EQ(Ansys::OK, bootStatus);
}

TEST_F(AnsysFixture, OneTask) {
}

TEST_F(AnsysFixture, TwoTasks) {
}
