#include <limits>

#include <unity.h>

#include "core/util/control/PidControl.h"
#include "helpers/AssertHelper.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

void test_pid_conditional_integration_can_recover_from_saturation(){
    // The negative proportional gain is deliberate: it creates a valid generic
    // PID state where the integral remains above the output ceiling while a
    // negative error should unwind it. The old implementation froze forever.
    PidControl pid(-1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    for(int i = 0; i < 6; ++i) pid.step(0.0f, 0.5f, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, pid.getOutput());

    for(int i = 0; i < 8; ++i) pid.step(0.25f, 0.0f, 1.0f);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, pid.getOutput());
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, pid.getOutput());
}

void test_pid_rejects_invalid_time_and_nonfinite_inputs(){
    TEST_RESET_ASSERTS();
    PidControl pid(1.0f, 0.1f, 0.1f, 0.0f, 1.0f);
    const float initial = pid.getOutput();

    TEST_ASSERT_EQUAL_FLOAT(initial, pid.step(0.0f, 1.0f, 0.0f));
    TEST_ASSERT_EQUAL_FLOAT(initial, pid.step(0.0f, 1.0f, -1.0f));
    TEST_ASSERT_EQUAL_FLOAT(initial, pid.step(
        std::numeric_limits<float>::quiet_NaN(), 1.0f, 1.0f));
    TEST_ASSERT_EQUAL_UINT32(3, AssertDebugCount);
}

} // namespace

void RegisterPidControlTests(){
    RUN_TEST(test_pid_conditional_integration_can_recover_from_saturation);
    RUN_TEST(test_pid_rejects_invalid_time_and_nonfinite_inputs);
}
