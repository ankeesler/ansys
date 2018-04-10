#include <thread>
#include <iostream>

#include "ansys_fixture.hpp"

static void taskB(volatile TestData *data) {
    data->output.taskBStarted = true;
    data->output.taskBFinished = true;
}

static void taskA(volatile TestData *data) {
    data->output.taskAStarted = true;
    if (data->input.runTaskB) {
        data->input.sys->CreateTask((void (*)(void *))taskB, (void *)data, 2);
    }
    data->output.taskAFinished = true;
}

static void bootTask(volatile TestData *data) {
    data->output.bootStarted = true;
    if (data->input.runTaskA) {
        data->input.sys->CreateTask((void (*)(void *))taskA, (void *)data, 1);
    }
    data->output.bootFinished = true;
}

static void sysRoutine(volatile TestData *data) {
    data->output.bootStatus = data->input.sys->Boot((void (*)(void *))bootTask, (void *)data);
    data->output.done = true;
}

void AnsysFixture::Start(bool runTaskA, bool runTaskB) {
    this->data.input.sys = &this->sys;
    this->data.input.runTaskA = runTaskA;
    this->data.input.runTaskB = runTaskB;

    this->data.output.bootStatus = Ansys::UNKNOWN;
    this->data.output.bootStarted = false;
    this->data.output.bootFinished = false;

    this->data.output.taskAStarted = false;
    this->data.output.taskAFinished = false;

    this->data.output.taskBStarted = false;
    this->data.output.taskBFinished = false;

    this->data.output.done = false;

    this->sys_thread = new std::thread(sysRoutine, &this->data);
}

void AnsysFixture::TearDown(void) {
    this->sys_thread->join();
    delete this->sys_thread;
}
