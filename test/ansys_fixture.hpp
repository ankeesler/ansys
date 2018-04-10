#include <thread>

#include "gtest/gtest.h"
#include "ansys.hpp"

struct TestInput {
    Ansys::Ansys *sys;
    bool runTaskA, runTaskB;
};

struct TestOutput {
    Ansys::Status bootStatus;
    bool bootStarted, bootFinished;

    bool taskAStarted, taskAFinished;

    bool taskBStarted, taskBFinished;

    bool done;
};

struct TestData {
    TestInput input;
    TestOutput output;
};

class AnsysFixture : public ::testing::Test {
    protected:
        volatile TestData data;

        void Start(bool runTaskA, bool runTaskB);

        virtual void TearDown(void);

    private:
        Ansys::Ansys sys;
        std::thread *sys_thread;
};
