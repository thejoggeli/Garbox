#include <unity.h>

// The runtime is the production owner of publish(). This visibility override
// lets the host test exercise the generated transition without widening the
// firmware API solely for testing.
#define private public
#include "generated/states/types/DisplayStatusState.h"
#undef private

using namespace Garbox;

namespace {

void test_generated_state_separates_pending_current_and_last(){
    DisplayStatusState state;

    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.lastBrightness());

    state.setBrightness(0.25f);

    TEST_ASSERT_TRUE(state.isDirty());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.lastBrightness());

    state.publish();

    TEST_ASSERT_FALSE(state.isDirty());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.lastBrightness());

    state.setBrightness(0.75f);
    state.publish();

    TEST_ASSERT_EQUAL_FLOAT(0.75f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.75f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.lastBrightness());
}

} // namespace

void RegisterStateTests(){
    RUN_TEST(test_generated_state_separates_pending_current_and_last);
}
