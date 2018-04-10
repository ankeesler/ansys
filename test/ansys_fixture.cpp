#include <thread>
#include <iostream>

#include "ansys_fixture.hpp"

static void bootTask(volatile TestData *data) {
    data->output.bootStarted = true;
    data->output.bootFinished = true;
}

static void sysRoutine(Ansys::Ansys& sys, volatile TestData *result) {
    result->output.bootStatus = sys.Boot((void (*)(void *))bootTask, (void *)result);
    result->output.done = true;
}

AnsysFixture::AnsysFixture(void) {
    this->data.output.bootStatus = Ansys::UNKNOWN;
    this->data.output.bootStarted = false;
    this->data.output.bootFinished = false;

    this->data.output.done = false;
}

void AnsysFixture::SetUp(void) {
    this->sys_thread = new std::thread(sysRoutine, std::ref(this->sys), &this->data);
}

void AnsysFixture::TearDown(void) {
    this->sys_thread->join();
    delete this->sys_thread;
}
