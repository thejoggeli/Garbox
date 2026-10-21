#include <cstdint>
#include <limits>

#include <unity.h>

#include "core/time/SoftwarePwm.h"
#include "core/time/SoftwareTimer.h"
#include "helpers/FakeTime.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

void tickAfter(SoftwarePwm& pwm, uint32_t micros){
    FakeTime::AdvanceMicros(micros);
    pwm.tick();
}

void test_timer_zero_duration_and_reset_contract(){
    FakeTime::Reset(100U);
    SoftwareTimer timer;

    TEST_ASSERT_TRUE(timer.isReset());
    TEST_ASSERT_FALSE(timer.isExpired());

    timer.start(0U);
    TEST_ASSERT_TRUE(timer.isRunning());
    TEST_ASSERT_TRUE(timer.isExpired());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, timer.getElapsedFraction());

    timer.reset();
    TEST_ASSERT_TRUE(timer.isReset());
    TEST_ASSERT_EQUAL_UINT32(0U, timer.getElapsedMicros());
}

void test_timer_expiry_and_restart_cross_uint32_wrap(){
    constexpr uint64_t start =
        static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) - 9ULL;
    FakeTime::Reset(start);

    SoftwareTimer timer;
    timer.start(20U);
    FakeTime::AdvanceMicros(15U);
    TEST_ASSERT_FALSE(timer.isExpired());

    FakeTime::AdvanceMicros(10U);
    TEST_ASSERT_TRUE(timer.isExpired());
    TEST_ASSERT_EQUAL_UINT32(25U, timer.getElapsedMicros());

    timer.restart();
    TEST_ASSERT_FALSE(timer.isExpired());
    TEST_ASSERT_EQUAL_UINT32(5U, timer.getElapsedMicros());
}

void test_timer_restart_skips_missed_periods_without_drift(){
    FakeTime::Reset(1'000U);
    SoftwareTimer timer;
    timer.start(100U);

    FakeTime::AdvanceMicros(375U);
    timer.restart();

    TEST_ASSERT_EQUAL_UINT32(75U, timer.getElapsedMicros());
    FakeTime::AdvanceMicros(25U);
    TEST_ASSERT_TRUE(timer.isExpired());
}

void test_timer_restart_handles_long_stall_in_constant_work(){
    FakeTime::Reset(123U);
    SoftwareTimer timer;
    timer.start(1'000U);

    FakeTime::AdvanceMicros(4'000'000'321ULL);
    timer.restart();

    TEST_ASSERT_EQUAL_UINT32(321U, timer.getElapsedMicros());
    TEST_ASSERT_FALSE(timer.isExpired());
}

void test_pwm_start_high_and_start_low_phase_boundaries(){
    FakeTime::Reset();
    SoftwarePwm startHigh(0.25f, 100U);
    startHigh.setMode(SoftwarePwm::Mode::StartHigh);
    startHigh.start();
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, startHigh.getState());
    tickAfter(startHigh, 24U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, startHigh.getState());
    tickAfter(startHigh, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, startHigh.getState());

    FakeTime::Reset();
    SoftwarePwm startLow(0.25f, 100U);
    startLow.setMode(SoftwarePwm::Mode::StartLow);
    startLow.start();
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, startLow.getState());
    tickAfter(startLow, 74U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, startLow.getState());
    tickAfter(startLow, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, startLow.getState());
}

void test_pwm_zero_and_full_duty_are_constant(){
    FakeTime::Reset();
    SoftwarePwm low(0.0f, 100U);
    low.start();
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, low.getState());
    tickAfter(low, 500U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, low.getState());

    FakeTime::Reset();
    SoftwarePwm high(1.0f, 100U);
    high.start();
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, high.getState());
    tickAfter(high, 500U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, high.getState());
}

void test_pwm_prestart_changes_define_first_cycle(){
    FakeTime::Reset();
    SoftwarePwm pwm(0.25f, 100U);
    pwm.setMode(SoftwarePwm::Mode::StartHigh);
    pwm.setDutyCycle(0.5f);
    pwm.setPeriodDurationMicros(200U);
    pwm.start();

    TEST_ASSERT_EQUAL_FLOAT(0.5f, pwm.getCurrentDutyCycle());
    TEST_ASSERT_EQUAL_UINT32(200U, pwm.getCurrentPeriodDurationMicros());
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());
    tickAfter(pwm, 99U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());
    tickAfter(pwm, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, pwm.getState());
}

void test_pwm_deferred_change_finishes_current_cycle(){
    FakeTime::Reset();
    SoftwarePwm pwm(0.5f, 100U);
    pwm.setMode(SoftwarePwm::Mode::StartHigh);
    pwm.start();

    tickAfter(pwm, 30U);
    pwm.setDutyCycle(0.2f, true);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, pwm.getCurrentDutyCycle());
    TEST_ASSERT_EQUAL_FLOAT(0.2f, pwm.getNextDutyCycle());

    tickAfter(pwm, 20U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, pwm.getState());
    tickAfter(pwm, 50U);
    TEST_ASSERT_EQUAL_FLOAT(0.2f, pwm.getCurrentDutyCycle());
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());
    tickAfter(pwm, 20U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, pwm.getState());
}

void test_pwm_immediate_change_starts_new_cycle_now(){
    FakeTime::Reset();
    SoftwarePwm pwm(0.5f, 100U);
    pwm.setMode(SoftwarePwm::Mode::StartHigh);
    pwm.start();

    tickAfter(pwm, 30U);
    pwm.setDutyCycle(0.2f, false);
    TEST_ASSERT_EQUAL_FLOAT(0.2f, pwm.getCurrentDutyCycle());
    TEST_ASSERT_EQUAL_UINT32(0U, pwm.getProgressMicros());
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());

    tickAfter(pwm, 19U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());
    tickAfter(pwm, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, pwm.getState());
}

void test_pwm_deferred_and_immediate_period_changes_have_distinct_boundaries(){
    FakeTime::Reset();
    SoftwarePwm deferred(0.5f, 100U);
    deferred.setMode(SoftwarePwm::Mode::StartHigh);
    deferred.start();
    tickAfter(deferred, 30U);
    deferred.setPeriodDurationMicros(200U, true);

    TEST_ASSERT_EQUAL_UINT32(100U, deferred.getCurrentPeriodDurationMicros());
    TEST_ASSERT_EQUAL_UINT32(200U, deferred.getNextPeriodDurationMicros());
    tickAfter(deferred, 70U);
    TEST_ASSERT_EQUAL_UINT32(200U, deferred.getCurrentPeriodDurationMicros());
    TEST_ASSERT_EQUAL_UINT32(0U, deferred.getProgressMicros());
    tickAfter(deferred, 99U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, deferred.getState());
    tickAfter(deferred, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, deferred.getState());

    FakeTime::Reset();
    SoftwarePwm immediate(0.5f, 100U);
    immediate.setMode(SoftwarePwm::Mode::StartHigh);
    immediate.start();
    tickAfter(immediate, 30U);
    immediate.setPeriodDurationMicros(200U, false);

    TEST_ASSERT_EQUAL_UINT32(200U, immediate.getCurrentPeriodDurationMicros());
    TEST_ASSERT_EQUAL_UINT32(0U, immediate.getProgressMicros());
    tickAfter(immediate, 99U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, immediate.getState());
    tickAfter(immediate, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, immediate.getState());
}

void test_pwm_stop_reconfigure_and_restart(){
    FakeTime::Reset();
    SoftwarePwm pwm(0.4f, 100U);
    pwm.setMode(SoftwarePwm::Mode::StartHigh);
    pwm.start();
    pwm.stop();

    TEST_ASSERT_TRUE(pwm.isReset());
    pwm.setDutyCycle(0.75f);
    pwm.setPeriodDurationMicros(200U);
    pwm.start();

    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());
    tickAfter(pwm, 149U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::High, pwm.getState());
    tickAfter(pwm, 1U);
    TEST_ASSERT_EQUAL(SoftwarePwm::State::Low, pwm.getState());
}

} // namespace

void RegisterTimerPwmTests(){
    RUN_TEST(test_timer_zero_duration_and_reset_contract);
    RUN_TEST(test_timer_expiry_and_restart_cross_uint32_wrap);
    RUN_TEST(test_timer_restart_skips_missed_periods_without_drift);
    RUN_TEST(test_timer_restart_handles_long_stall_in_constant_work);
    RUN_TEST(test_pwm_start_high_and_start_low_phase_boundaries);
    RUN_TEST(test_pwm_zero_and_full_duty_are_constant);
    RUN_TEST(test_pwm_prestart_changes_define_first_cycle);
    RUN_TEST(test_pwm_deferred_change_finishes_current_cycle);
    RUN_TEST(test_pwm_immediate_change_starts_new_cycle_now);
    RUN_TEST(test_pwm_deferred_and_immediate_period_changes_have_distinct_boundaries);
    RUN_TEST(test_pwm_stop_reconfigure_and_restart);
}
