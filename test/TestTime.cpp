#include <cstdint>
#include <limits>

#include <unity.h>

#include "core/time/Time.h"
#include "helpers/FakeTime.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

void test_fake_time_controls_tick_snapshots(){
    FakeTime::Reset(1'250'000ULL);
    Time::Start();

    TEST_ASSERT_EQUAL_UINT32(1'250'000U, Time::GetTickMicros());
    TEST_ASSERT_EQUAL_UINT32(1'250U, Time::GetTickMillis());
    TEST_ASSERT_EQUAL_UINT32(1U, Time::GetTickSeconds());
    TEST_ASSERT_EQUAL_UINT32(0U, Time::GetTickDeltaMicros());

    FakeTime::AdvanceMicros(750U);
    Time::Tick();

    TEST_ASSERT_EQUAL_UINT32(1'250'750U, Time::GetTickMicros());
    TEST_ASSERT_EQUAL_UINT32(750U, Time::GetTickDeltaMicros());
    TEST_ASSERT_FLOAT_WITHIN(0.000001f, 0.000750f, Time::GetTickDeltaSeconds());
}

void test_fake_time_exposes_modular_microsecond_wrap(){
    constexpr uint64_t start =
        static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) - 9ULL;

    FakeTime::Reset(start);
    Time::Start();
    FakeTime::AdvanceMicros(25U);
    Time::Tick();

    TEST_ASSERT_EQUAL_UINT32(15U, Time::GetTickMicros());
    TEST_ASSERT_EQUAL_UINT32(25U, Time::GetTickDeltaMicros());
}

void test_fake_delays_advance_without_wall_clock_sleep(){
    FakeTime::Reset();

    Time::DelayMillis(2U);
    Time::BlockMillis(3U);
    Time::BlockMicros(400U);

    TEST_ASSERT_EQUAL_UINT64(5'400ULL, FakeTime::GetMicros64());
}

} // namespace

void RegisterTimeTests(){
    RUN_TEST(test_fake_time_controls_tick_snapshots);
    RUN_TEST(test_fake_time_exposes_modular_microsecond_wrap);
    RUN_TEST(test_fake_delays_advance_without_wall_clock_sleep);
}
