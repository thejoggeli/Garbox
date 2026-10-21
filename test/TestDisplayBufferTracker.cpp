#include <cstdint>

#include <unity.h>

#include "core/lvgl/helpers/DisplayBufferTracker.h"

using namespace Garbox;

namespace {

using Result = DisplayBufferTracker::Result;
using State = DisplayBufferTracker::State;

void assertResult(Result expected, Result actual){
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(expected), static_cast<uint8_t>(actual));
}

void assertState(State expected, State actual){
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(expected), static_cast<uint8_t>(actual));
}

void registerThree(DisplayBufferTracker& tracker, uint8_t& a, uint8_t& b, uint8_t& c){
    assertResult(Result::Ok, tracker.registerBuffer(&a));
    assertResult(Result::Ok, tracker.registerBuffer(&b));
    assertResult(Result::Ok, tracker.registerBuffer(&c));
    TEST_ASSERT_EQUAL_UINT32(3U, tracker.registeredCount());
}

void test_display_buffers_support_three_in_flight_and_ordered_completion(){
    DisplayBufferTracker tracker;
    uint8_t a = 0U;
    uint8_t b = 0U;
    uint8_t c = 0U;
    registerThree(tracker, a, b, c);

    assertResult(Result::Ok, tracker.queue(&a));
    assertResult(Result::Ok, tracker.queue(&b));
    assertResult(Result::Ok, tracker.queue(&c));
    TEST_ASSERT_EQUAL_UINT32(3U, tracker.queuedCount());

    assertResult(Result::OutOfOrder, tracker.beginDma(&b));
    assertResult(Result::Ok, tracker.beginDma(&a));
    TEST_ASSERT_EQUAL_PTR(&a, tracker.dmaBuffer());
    assertState(State::DmaOwned, tracker.stateOf(&a));

    assertResult(Result::DmaBusy, tracker.beginDma(&b));
    assertResult(Result::WrongDmaCompletion, tracker.completeDma(&b));
    assertResult(Result::Ok, tracker.completeDma(&a));
    assertResult(Result::Ok, tracker.beginDma(&b));
    assertResult(Result::Ok, tracker.completeDma(&b));
    assertResult(Result::Ok, tracker.beginDma(&c));
    assertResult(Result::Ok, tracker.completeDma(&c));
    TEST_ASSERT_EQUAL_UINT32(0U, tracker.queuedCount());
}

void test_display_buffer_rejects_duplicate_and_wrong_transitions(){
    DisplayBufferTracker tracker;
    uint8_t a = 0U;
    uint8_t b = 0U;
    uint8_t c = 0U;
    uint8_t unknown = 0U;
    registerThree(tracker, a, b, c);

    assertResult(Result::DuplicateBuffer, tracker.registerBuffer(&a));
    assertResult(Result::CapacityExceeded, tracker.registerBuffer(&unknown));
    assertResult(Result::UnknownBuffer, tracker.queue(&unknown));
    assertResult(Result::InvalidTransition, tracker.beginDma(&a));

    assertResult(Result::Ok, tracker.queue(&a));
    assertResult(Result::InvalidTransition, tracker.queue(&a));
    assertResult(Result::Ok, tracker.beginDma(&a));
    assertResult(Result::WrongDmaCompletion, tracker.completeDma(&b));
    assertResult(Result::Ok, tracker.completeDma(&a));
    assertResult(Result::InvalidTransition, tracker.completeDma(&a));
}

void test_display_buffer_can_be_reused_for_successive_partial_flushes(){
    DisplayBufferTracker tracker;
    uint8_t a = 0U;
    uint8_t b = 0U;
    uint8_t c = 0U;
    registerThree(tracker, a, b, c);

    for(uint32_t partialFlush = 0U; partialFlush < 4U; ++partialFlush){
        assertResult(Result::Ok, tracker.queue(&a));
        assertState(State::Queued, tracker.stateOf(&a));
        assertResult(Result::Ok, tracker.beginDma(&a));
        assertResult(Result::Ok, tracker.completeDma(&a));
        assertState(State::LvglOwned, tracker.stateOf(&a));
    }
}

void test_display_buffer_queue_rollback_preserves_fifo(){
    DisplayBufferTracker tracker;
    uint8_t a = 0U;
    uint8_t b = 0U;
    uint8_t c = 0U;
    registerThree(tracker, a, b, c);

    assertResult(Result::Ok, tracker.queue(&a));
    assertResult(Result::Ok, tracker.queue(&b));
    assertResult(Result::OutOfOrder, tracker.cancelQueued(&a));
    assertResult(Result::Ok, tracker.cancelQueued(&b));
    assertState(State::LvglOwned, tracker.stateOf(&b));
    assertResult(Result::Ok, tracker.beginDma(&a));
    assertResult(Result::Ok, tracker.completeDma(&a));
}

} // namespace

void RegisterDisplayBufferTrackerTests(){
    RUN_TEST(test_display_buffers_support_three_in_flight_and_ordered_completion);
    RUN_TEST(test_display_buffer_rejects_duplicate_and_wrong_transitions);
    RUN_TEST(test_display_buffer_can_be_reused_for_successive_partial_flushes);
    RUN_TEST(test_display_buffer_queue_rollback_preserves_fifo);
}
