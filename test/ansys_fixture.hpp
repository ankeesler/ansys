#include <thread>

#include "gtest/gtest.h"
#include "ansys.hpp"

struct TestInput {
};


struct TestOutput {
    Ansys::Status bootStatus;
    bool bootStarted, bootFinished;

    bool done;
};

struct TestData {
    TestInput input;
    TestOutput output;
};

class AnsysFixture : public ::testing::Test {
    public:
        AnsysFixture(void);

    protected:
        volatile TestData data;

        virtual void SetUp(void);
        virtual void TearDown(void);

    private:
        Ansys::Ansys sys;
        std::thread *sys_thread;
};
