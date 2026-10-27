#include <unity.h>

#include <cmath>
#include <limits>
#include <type_traits>

#include "helpers/AssertHelper.h"
#include "core/util/color/ColorMap.h"
#include "core/util/function/filter/composite/CompositeFilter.h"
#include "core/util/function/filter/filters/MovingAverageFilter.h"
#include "core/util/function/default/MathFunctions.h"
#include "core/util/function/math/custom/AnalyticFunction.h"
#include "core/util/function/math/custom/ConstantFunction.h"
#include "core/util/function/math/custom/LookupFunction.h"
#include "core/util/function/math/custom/SampledFunction.h"
#include "core/util/function/math/standard/LinearFunction.h"
#include "core/util/function/math/standard/QuadraticFunction.h"
#include "core/util/function/math/standard/ScaleFunction.h"

using namespace Garbox;
using namespace GarboxTest;

static_assert(!std::is_copy_constructible_v<MovingAverageFilter>);
static_assert(!std::is_copy_constructible_v<CompositeFilter>);
static_assert(!std::is_copy_constructible_v<LookupFunction>);
static_assert(!std::is_copy_constructible_v<SampledFunction>);

namespace {

void test_moving_average_owns_heap_and_uses_wide_accumulator(){
    TEST_RESET_ASSERTS();
    {
        MovingAverageFilter filter(4, 1000.0f);
        filter.reset(2'000'000.0f);
        TEST_ASSERT_FLOAT_WITHIN(1.0f, 2'000'000.0f, filter.process(2'000'000.0f));
        TEST_ASSERT_FLOAT_WITHIN(1.0f, 2'000'000.0f, filter.process(2'000'000.0f));
        TEST_ASSERT_FLOAT_WITHIN(1.0f, 2'000'000.0f, filter.process(2'000'000.0f));

        filter.setScaleFactor(500.0f);
        TEST_ASSERT_FLOAT_WITHIN(1.0f, 2'000'000.0f, filter.getFilteredValue());
    }
    TEST_ASSERT_NO_ASSERTS();
}

void test_moving_average_rejects_invalid_scale_and_input_without_corruption(){
    TEST_RESET_ASSERTS();
    MovingAverageFilter filter(2, 10.0f);
    filter.reset(2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, filter.process(4.0f));

    filter.setScaleFactor(-1.0f);
    TEST_ASSERT_EXIT_CALLED();
    TEST_RESET_ASSERTS();

    const float before = filter.getFilteredValue();
    TEST_ASSERT_FLOAT_WITHIN(
        0.001f,
        before,
        filter.process(std::numeric_limits<float>::infinity())
    );
    TEST_ASSERT_EXIT_CALLED();
}

void test_composite_filter_releases_owned_arrays_and_handles_null_stage(){
    TEST_RESET_ASSERTS();
    ScaleFunction doubleValue(2.0f);
    ScaleFunction halfValue(0.5f);
    {
        CompositeFilter filter({&doubleValue, &halfValue}, true);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, filter.process(7.0f));
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 14.0f, filter.getResult(0));
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, filter.getResult(1));
    }
    TEST_ASSERT_NO_ASSERTS();

    TEST_RESET_ASSERTS();
    FunctionIfc* stages[] = {&doubleValue, nullptr, &halfValue};
    CompositeFilter filter(stages, nullptr, 3);
    TEST_ASSERT_EXIT_CALLED();
    TEST_RESET_ASSERTS();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, filter.process(7.0f));
    TEST_ASSERT_EXIT_CALLED();
}

void test_lookup_function_owns_copy_and_rejects_nonfinite_data(){
    TEST_RESET_ASSERTS();
    {
        LookupFunction function;
        function.init({{0.0f, 0.0f}, {1.0f, 10.0f}, {2.0f, 20.0f}});
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 15.0f, function.evaluate(1.5f));
    }
    TEST_ASSERT_NO_ASSERTS();

    TEST_RESET_ASSERTS();
    LookupFunction invalid;
    invalid.init({{0.0f, 0.0f}, {std::numeric_limits<float>::quiet_NaN(), 1.0f}});
    TEST_ASSERT_EXIT_CALLED();
}

void test_sampled_function_owns_copy_and_validates_domain(){
    TEST_RESET_ASSERTS();
    {
        SampledFunction function;
        function.init({0.0f, 10.0f, 20.0f}, 0.0f, 2.0f);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 15.0f, function.evaluate(1.5f));
    }
    TEST_ASSERT_NO_ASSERTS();

    TEST_RESET_ASSERTS();
    SampledFunction invalid;
    invalid.init({1.0f}, 0.0f, 1.0f);
    TEST_ASSERT_EXIT_CALLED();
}

void test_color_map_is_non_owning_and_destructs_normally(){
    TEST_RESET_ASSERTS();
    static const ColorMap::Entry entries[] = {
        {0.0f, RgbFloat(0.0f, 0.0f, 0.0f)},
        {1.0f, RgbFloat(1.0f, 1.0f, 1.0f)},
    };
    {
        ColorMap map{Span<const ColorMap::Entry>(entries)};
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, map.interpolateStandardRgb(0.5f).r);
    }
    TEST_ASSERT_NO_ASSERTS();
}

void test_math_function_factories_match_their_contracts(){
    TEST_RESET_ASSERTS();
    ConstantFunction constant(7.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, constant.evaluate(-123.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.002f, 0.5f, MathFunctions::GetSqrt().evaluate(0.25f));
    TEST_ASSERT_FLOAT_WITHIN(0.002f, 1.0f, MathFunctions::GetSin90().evaluate(0.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.002f, 0.0f, MathFunctions::GetSin180().evaluate(0.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.002f, -1.0f, MathFunctions::GetSin270().evaluate(0.0f));
    TEST_ASSERT_NO_ASSERTS();
}

void test_invalid_function_initialization_preserves_safe_state(){
    TEST_RESET_ASSERTS();
    AnalyticFunction analytic;
    analytic.init(nullptr);
    TEST_ASSERT_EXIT_CALLED();
    TEST_RESET_ASSERTS();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, analytic.evaluate(2.0f));
    TEST_ASSERT_DEBUG_CALLED();

    TEST_RESET_ASSERTS();
    LinearFunction linear(2.0f, 3.0f);
    linear.setPoints(1.0f, 2.0f, 1.0f, 4.0f);
    TEST_ASSERT_EXIT_CALLED();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, linear.evaluate(2.0f));

    TEST_RESET_ASSERTS();
    QuadraticFunction quadratic(1.0f, 2.0f, 3.0f);
    quadratic.setPoints(1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f);
    TEST_ASSERT_EXIT_CALLED();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 11.0f, quadratic.evaluate(2.0f));
}

} // namespace

void RegisterFunctionTests(){
    RUN_TEST(test_moving_average_owns_heap_and_uses_wide_accumulator);
    RUN_TEST(test_moving_average_rejects_invalid_scale_and_input_without_corruption);
    RUN_TEST(test_composite_filter_releases_owned_arrays_and_handles_null_stage);
    RUN_TEST(test_lookup_function_owns_copy_and_rejects_nonfinite_data);
    RUN_TEST(test_sampled_function_owns_copy_and_validates_domain);
    RUN_TEST(test_color_map_is_non_owning_and_destructs_normally);
    RUN_TEST(test_math_function_factories_match_their_contracts);
    RUN_TEST(test_invalid_function_initialization_preserves_safe_state);
}
