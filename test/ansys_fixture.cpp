#include <thread>
#include <iostream>

#include "ansys_fixture.hpp"

struct TestInput {
    Ansys::Ansys sys;
    bool runTaskA, runTaskB;
};

struct TestOutput {
    Ansys::Status bootStatus;
    std::queue<TestEvent> events;
};

struct TestData {
    TestInput input;
    TestOutput output;
};

static void taskB(TestData *data) {
    data->output.events.push(TestEvent::TASK_B_STARTED);
    data->input.sys.Yield();
    data->output.events.push(TestEvent::TASK_B_ENDED);
}

static void taskA(TestData *data) {
    data->output.events.push(TestEvent::TASK_A_STARTED);
    if (data->input.runTaskB) {
        data->input.sys.CreateTask((void (*)(void *))taskB, (void *)data, 1);
        data->output.events.push(TestEvent::TASK_A_CONTINUED);
        data->input.sys.Yield();
    }
    data->output.events.push(TestEvent::TASK_A_ENDED);
}

static void bootTask(TestData *data) {
    data->output.events.push(TestEvent::BOOT_STARTED);
    if (data->input.runTaskA) {
        data->input.sys.CreateTask((void (*)(void *))taskA, (void *)data, 2);
    }
    data->output.events.push(TestEvent::BOOT_ENDED);
}

void AnsysFixture::Run(bool runTaskA, bool runTaskB) {
    TestData data;
    data.input.runTaskA = runTaskA;
    data.input.runTaskB = runTaskB;

    data.output.bootStatus = Ansys::UNKNOWN;
    data.output.bootStatus = data.input.sys.Boot((void (*)(void *))bootTask, (void *)&data);

    this->bootStatus = data.output.bootStatus;
    this->events = data.output.events;
}
