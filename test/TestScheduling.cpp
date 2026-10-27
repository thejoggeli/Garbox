#include <unity.h>

#include "helpers/AssertHelper.h"
#include "helpers/FakeTime.h"
#include "core/time/Time.h"
#include "core/util/helpers/DirtyDispatcher.h"
#include "core/util/scheduling/TimeSlotScheduler.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

uint32_t sSlotRuns = 0;

void countSlot(){
    ++sSlotRuns;
}

struct RequeueContext {
    DirtyDispatcher* dispatcher;
    uint32_t calls;
};

void requeueSelf(void* rawContext){
    auto& context = *static_cast<RequeueContext*>(rawContext);
    ++context.calls;
    context.dispatcher->markDirty(0);
}

void test_dirty_dispatcher_validates_registration_and_defers_callback_requeue(){
    TEST_RESET_ASSERTS();
    DirtyDispatcher dispatcher(1);
    RequeueContext context {&dispatcher, 0};
    dispatcher.registerHandler(requeueSelf, &context);
    TEST_ASSERT_NO_ASSERTS();

    dispatcher.dispatch();
    TEST_ASSERT_EQUAL_UINT32(1U, context.calls);
    TEST_ASSERT_EQUAL_UINT32(1U, dispatcher.getDirtyCount());
    TEST_ASSERT_TRUE(dispatcher.isMarkedDirty(0));

    dispatcher.dispatch();
    TEST_ASSERT_EQUAL_UINT32(2U, context.calls);
    TEST_ASSERT_EQUAL_UINT32(1U, dispatcher.getDirtyCount());

    TEST_RESET_ASSERTS();
    dispatcher.registerHandler(requeueSelf, &context);
    dispatcher.markDirty(4);
    TEST_ASSERT_FALSE(dispatcher.isMarkedDirty(4));
    TEST_ASSERT_EXIT_CALLED();
}

void test_empty_scheduler_is_inert_and_invalid_diagnostics_are_safe(){
    TEST_RESET_ASSERTS();
    TimeSlotScheduler scheduler({});
    TEST_ASSERT_EXIT_CALLED();
    scheduler.run();
    TEST_ASSERT_EQUAL_UINT32(0U, scheduler.getSlotCount());

    TEST_RESET_ASSERTS();
    const auto& diagnostics = scheduler.getDiagnostics(0);
    TEST_ASSERT_EQUAL_UINT32(0U, diagnostics.frequencyHz);
    TEST_ASSERT_EXIT_CALLED();
}

void test_scheduler_frequency_uses_actual_elapsed_window(){
    TEST_RESET_ASSERTS();
    FakeTime::Reset();
    Time::Start();
    sSlotRuns = 0;
    TimeSlotScheduler scheduler({{0U, 0U, countSlot}});

    scheduler.run();
    FakeTime::AdvanceMicros(1'500'000U);
    scheduler.run();

    TEST_ASSERT_EQUAL_UINT32(2U, sSlotRuns);
    TEST_ASSERT_EQUAL_UINT32(1U, scheduler.getDiagnostics(0).frequencyHz);
    TEST_ASSERT_NO_ASSERTS();
}

} // namespace

void RegisterSchedulingTests(){
    RUN_TEST(test_dirty_dispatcher_validates_registration_and_defers_callback_requeue);
    RUN_TEST(test_empty_scheduler_is_inert_and_invalid_diagnostics_are_safe);
    RUN_TEST(test_scheduler_frequency_uses_actual_elapsed_window);
}
