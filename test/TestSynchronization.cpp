#include <cstdint>

#include <unity.h>

#include "core/diagnostics/Profiler.h"
#include "core/rtos/MutexGuard.h"
#include "freertos/semphr.h"
#include "helpers/AssertHelper.h"
#include "helpers/FakeTime.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

void test_mutex_guards_use_matching_freertos_apis(){
    using namespace GarboxTest::FakeSemaphore;

    SemaphoreHandle_t normal = xSemaphoreCreateMutex();
    ResetCallCounts();
    {
        MutexGuard guard(normal);
        TEST_ASSERT_EQUAL_UINT32(1U, normal->depth);
    }
    TEST_ASSERT_EQUAL_UINT32(1U, Calls.normalTake);
    TEST_ASSERT_EQUAL_UINT32(1U, Calls.normalGive);
    TEST_ASSERT_EQUAL_UINT32(0U, Calls.recursiveTake);
    TEST_ASSERT_EQUAL_UINT32(0U, Calls.recursiveGive);

    SemaphoreHandle_t recursive = xSemaphoreCreateRecursiveMutex();
    ResetCallCounts();
    {
        RecursiveMutexGuard outer(recursive);
        RecursiveMutexGuard inner(recursive);
        TEST_ASSERT_EQUAL_UINT32(2U, recursive->depth);
    }
    TEST_ASSERT_EQUAL_UINT32(0U, recursive->depth);
    TEST_ASSERT_EQUAL_UINT32(0U, Calls.normalTake);
    TEST_ASSERT_EQUAL_UINT32(0U, Calls.normalGive);
    TEST_ASSERT_EQUAL_UINT32(2U, Calls.recursiveTake);
    TEST_ASSERT_EQUAL_UINT32(2U, Calls.recursiveGive);
}

void test_mutex_guard_reports_mismatched_mutex_kind(){
    TEST_RESET_ASSERTS();
    SemaphoreHandle_t recursive = xSemaphoreCreateRecursiveMutex();
    {
        MutexGuard wrongGuard(recursive);
    }
    TEST_ASSERT_EQUAL_UINT32(0U, AssertDebugCount);
    TEST_ASSERT_EQUAL_UINT32(1U, AssertExitCount);
}

void test_profiler_uses_one_normal_lock_for_reset_and_returns_snapshots(){
    using namespace GarboxTest::FakeSemaphore;

    TEST_RESET_ASSERTS();
    FakeTime::Reset();
    TEST_ASSERT_TRUE(Profiler::Init());
    Profiler::SetEnabled(true);

    Profiler::MeasureBegin(ProfilerId::MainBusy);
    FakeTime::AdvanceMicros(125U);
    Profiler::MeasureEnd(ProfilerId::MainBusy);
    FakeTime::AdvanceMicros(875U);
    Profiler::UpdateAll();

    const Profiler::Record snapshot = Profiler::GetRecord(ProfilerId::MainBusy);
    TEST_ASSERT_EQUAL_UINT32(1U, snapshot.countLast);
    TEST_ASSERT_EQUAL_UINT32(125U, snapshot.minDurationLast);
    TEST_ASSERT_EQUAL_UINT32(125U, snapshot.maxDurationLast);

    ResetCallCounts();
    Profiler::Reset();
    TEST_ASSERT_EQUAL_UINT32(1U, Calls.normalTake);
    TEST_ASSERT_EQUAL_UINT32(1U, Calls.normalGive);
    TEST_ASSERT_EQUAL_UINT32(0U, Calls.recursiveTake);
    TEST_ASSERT_EQUAL_UINT32(0U, Calls.recursiveGive);
    TEST_ASSERT_NO_ASSERTS();
}

} // namespace

void RegisterSynchronizationTests(){
    RUN_TEST(test_mutex_guards_use_matching_freertos_apis);
    RUN_TEST(test_mutex_guard_reports_mismatched_mutex_kind);
    RUN_TEST(test_profiler_uses_one_normal_lock_for_reset_and_returns_snapshots);
}
