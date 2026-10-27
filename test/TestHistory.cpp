#include <cstdint>
#include <limits>
#include <vector>

#include <unity.h>

// Keep the visibility override local to TimeSeries itself; its dependency
// headers are parsed first so standard/library internals are unaffected.
#include "core/util/container/Span.h"
#include "core/util/container/heap/ArrayHeap.h"
#define private public
#include "core/util/history/TimeSeries.h"
#undef private

#include "core/time/Time.h"
#include "helpers/FakeTime.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

void advanceAndTick(TimeSeries& series, uint64_t micros, int32_t value){
    FakeTime::AdvanceMicros(micros);
    Time::Tick();
    series.tick(value);
}

std::vector<int32_t> collect(TimeSeries::IteratorSinceSequence iterator){
    std::vector<int32_t> values;
    while(iterator.hasNext()) values.push_back(iterator.next());
    return values;
}

void assertValues(const std::vector<int32_t>& actual, std::initializer_list<int32_t> expected){
    TEST_ASSERT_EQUAL_UINT32(expected.size(), actual.size());
    std::size_t index = 0;
    for(int32_t value : expected){
        TEST_ASSERT_EQUAL_INT32(value, actual[index]);
        ++index;
    }
}

void test_four_day_window_uses_64_bit_sample_boundaries(){
    constexpr uint32_t FourDaysSeconds = 4U * 24U * 60U * 60U;
    FakeTime::Reset();
    Time::Start();
    TimeSeries series(FourDaysSeconds, 65, TimeSeries::SamplingMode::Average);

    const uint64_t interval = series.getSampleIntervalMicros();
    TEST_ASSERT_EQUAL_UINT64(5'316'923'076ULL, interval);

    series.tick(std::numeric_limits<int32_t>::max());
    advanceAndTick(series, interval - 1U, std::numeric_limits<int32_t>::max());
    TEST_ASSERT_EQUAL_UINT16(1, series.getAvailableSize());
    advanceAndTick(series, 1U, std::numeric_limits<int32_t>::max());

    TEST_ASSERT_EQUAL_UINT16(2, series.getAvailableSize());
    assertValues(collect(series.iterateSince(0)), {
        std::numeric_limits<int32_t>::max(),
        std::numeric_limits<int32_t>::max()
    });
}

void test_average_sum_and_count_survive_long_high_rate_bucket(){
    FakeTime::Reset();
    Time::Start();
    TimeSeries series(1, 1, TimeSeries::SamplingMode::Average);
    series.tick(100'000);

    for(uint32_t i = 0; i < 70'000; ++i) series.tick(100'000);
    advanceAndTick(series, 1'000'000ULL, 100'000);

    assertValues(collect(series.iterateSince(1)), {100'000});
}

void test_since_iterator_clamps_consumer_overrun_to_retained_ring(){
    FakeTime::Reset();
    Time::Start();
    TimeSeries series(3, 3, TimeSeries::SamplingMode::Latest);
    series.tick(1);
    advanceAndTick(series, 1'000'000ULL, 2);
    advanceAndTick(series, 1'000'000ULL, 3);
    advanceAndTick(series, 1'000'000ULL, 4);

    auto overrun = series.iterateSince(0);
    TEST_ASSERT_EQUAL_UINT32(4, overrun.getNewWriteSequence());
    assertValues(collect(overrun), {2, 3, 4});
    assertValues(collect(series.iterateSince(2)), {3, 4});
    assertValues(collect(series.iterateSince(4)), {});
}

void test_since_iterator_handles_sequence_wrap_without_replaying(){
    FakeTime::Reset();
    Time::Start();
    TimeSeries series(3, 3, TimeSeries::SamplingMode::Latest);
    series.tick(10);
    series.mWriteSequence = std::numeric_limits<uint32_t>::max() - 1U;

    advanceAndTick(series, 1'000'000ULL, 20);
    advanceAndTick(series, 1'000'000ULL, 30);
    TEST_ASSERT_EQUAL_UINT32(0, series.getWriteSequence());

    auto wrapped = series.iterateSince(std::numeric_limits<uint32_t>::max() - 1U);
    TEST_ASSERT_EQUAL_UINT32(0, wrapped.getNewWriteSequence());
    assertValues(collect(wrapped), {20, 30});
}

} // namespace

void RegisterHistoryTests(){
    RUN_TEST(test_four_day_window_uses_64_bit_sample_boundaries);
    RUN_TEST(test_average_sum_and_count_survive_long_high_rate_bucket);
    RUN_TEST(test_since_iterator_clamps_consumer_overrun_to_retained_ring);
    RUN_TEST(test_since_iterator_handles_sequence_wrap_without_replaying);
}
