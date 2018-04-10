#include <thread>
#include <iostream>

#include "ansys_fixture.hpp"

static void bootTask(volatile TestResult *input) {
    input->bootStarted = true;
    input->bootFinished = true;
}

static void sysRoutine(Ansys::Ansys& sys, volatile TestResult *result) {
    result->bootStatus = sys.Boot((void (*)(void *))bootTask, (void *)result);
    result->done = true;
}

void AnsysFixture::SetUp(void) {
    this->sys_thread = new std::thread(sysRoutine, std::ref(this->sys), &this->result);
}

void AnsysFixture::TearDown(void) {
    this->sys_thread->join();
    delete this->sys_thread;
}
