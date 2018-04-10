#include <thread>

#include "gtest/gtest.h"
#include "ansys.hpp"

class TestResult {
    public:
        TestResult(void) : bootStatus(Ansys::UNKNOWN), done(false) { }

        Ansys::Status bootStatus;
        bool bootStarted;
        bool bootFinished;
        bool done;
};

class AnsysFixture : public ::testing::Test {
    protected:
        volatile TestResult result;

        virtual void SetUp(void);
        virtual void TearDown(void);

    private:
        Ansys::Ansys sys;
        std::thread *sys_thread;
};
