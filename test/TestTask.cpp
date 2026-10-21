#include <unity.h>

#include "core/rtos/Task.h"
#include "freertos/task.h"
#include "helpers/AssertHelper.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

void configureTask(Task& task, bool& resourceReleased){
    task.configure("native-task", 1024U, 1U, 0);
    task.setHandler([&resourceReleased](const TaskStopToken& stopToken){
        bool resourceOwned = true;
        if(stopToken.stopRequested()){
            resourceOwned = false;
            resourceReleased = true;
        }
        TEST_ASSERT_FALSE(resourceOwned);
    });
}

void test_external_stop_waits_for_worker_cleanup_without_deleting_it(){
    FakeTask::Reset();
    TEST_RESET_ASSERTS();
    bool resourceReleased = false;
    Task task;
    configureTask(task, resourceReleased);
    task.start();
    TaskHandle_t handle = task.getHandle();

    TEST_ASSERT_FALSE(task.stop(0U));
    TEST_ASSERT_TRUE(task.isRunning());
    TEST_ASSERT_EQUAL_UINT32(0U, FakeTask::ExternalDeleteCount);

    FakeTask::Run(handle);
    TEST_ASSERT_TRUE(resourceReleased);
    TEST_ASSERT_FALSE(task.isRunning());
    TEST_ASSERT_EQUAL_UINT32(1U, FakeTask::SelfDeleteCount);
    TEST_ASSERT_EQUAL_UINT32(0U, FakeTask::ExternalDeleteCount);
    TEST_ASSERT_TRUE(task.stop(0U));
    TEST_ASSERT_NO_ASSERTS();
}

void test_current_task_stop_is_request_only_and_unwinds_in_handler(){
    FakeTask::Reset();
    TEST_RESET_ASSERTS();
    bool resourceReleased = false;
    Task task;
    configureTask(task, resourceReleased);
    task.start();
    TaskHandle_t handle = task.getHandle();

    FakeTask::SetCurrent(handle);
    TEST_ASSERT_TRUE(task.isCurrentTask());
    TEST_ASSERT_FALSE(task.stop(0U));
    TEST_ASSERT_EQUAL_UINT32(0U, FakeTask::ExternalDeleteCount);
    FakeTask::SetCurrent(nullptr);

    FakeTask::Run(handle);
    TEST_ASSERT_TRUE(resourceReleased);
    TEST_ASSERT_FALSE(task.isRunning());
    TEST_ASSERT_EQUAL_UINT32(1U, FakeTask::SelfDeleteCount);
    TEST_ASSERT_EQUAL_UINT32(0U, FakeTask::ExternalDeleteCount);
    TEST_ASSERT_NO_ASSERTS();
}

} // namespace

void RegisterTaskTests(){
    RUN_TEST(test_external_stop_waits_for_worker_cleanup_without_deleting_it);
    RUN_TEST(test_current_task_stop_is_request_only_and_unwinds_in_handler);
}
