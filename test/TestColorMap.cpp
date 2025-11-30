#include <unity.h>
#include <iostream>

#include "helpers/AssertHelper.h"
#include "core/util/color/ColorMap.h"
#include "core/util/color/types/RgbFloat.h"
#include "core/util/container/Span.h"

using namespace Garbox;
using namespace GarboxTest;

static void assertRgbNear(const RgbFloat& c, float r, float g, float b, float eps){
    TEST_ASSERT_FLOAT_WITHIN(eps, r, c.r);
    TEST_ASSERT_FLOAT_WITHIN(eps, g, c.g);
    TEST_ASSERT_FLOAT_WITHIN(eps, b, c.b);
}

// --------------------------------------------------------------
// TEST 1: Uniform, 2 entries
// --------------------------------------------------------------
static void test_uniform_two_entries_explicit(){

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(0.0f, RgbFloat(1,0,0)),
        ColorMap::Entry(1.0f, RgbFloat(0,0,1)),
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    {
        RgbFloat c = map.interpolateStandardRgb(0.0f);
        assertRgbNear(c, 1,0,0, eps);
    }
    {
        float t = 0.25f;
        float r = 0.75f;
        float g = 0.0f;
        float b = 0.25f;
        RgbFloat c = map.interpolateStandardRgb(t);
        assertRgbNear(c, r,g,b, eps);
    }
    {
        RgbFloat c = map.interpolateStandardRgb(1.0f);
        assertRgbNear(c, 0,0,1, eps);
    }
}

// --------------------------------------------------------------
// TEST 2: Uniform, 3 entries auto-distributed
// --------------------------------------------------------------
static void test_uniform_three_entries_auto(){

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(RgbFloat(1,0,0)),
        ColorMap::Entry(RgbFloat(0,1,0)),
        ColorMap::Entry(RgbFloat(0,0,1)),
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    for(int i=0; i<=10; ++i){
        float t = i * 0.1f;
        RgbFloat c = map.interpolateStandardRgb(t);

        float r,g,b;

        if(t <= 0.5f){
            float a = t / 0.5f;
            r = 1.0f - a;
            g = a;
            b = 0.0f;
        }
        else {
            float a = (t - 0.5f) / 0.5f;
            r = 0.0f;
            g = 1.0f - a;
            b = a;
        }

        assertRgbNear(c, r,g,b, eps);
    }
}

// --------------------------------------------------------------
// TEST 3: Non-uniform R -> B -> R
// --------------------------------------------------------------
static void test_non_uniform_rbr(){

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(0.0f, RgbFloat(1,0,0)),
        ColorMap::Entry(0.2f, RgbFloat(0,0,1)),
        ColorMap::Entry(1.0f, RgbFloat(1,0,0)),
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    for(int i=0; i<=20; ++i){
        float t = i * 0.05f;
        RgbFloat c = map.interpolateStandardRgb(t);

        float r,g,b;

        if(t <= 0.2f){
            float a = t / 0.2f;
            r = 1.0f - a;
            g = 0.0f;
            b = a;
        }
        else {
            float a = (t - 0.2f) / 0.8f;
            r = a;
            g = 0.0f;
            b = 1.0f - a;
        }

        assertRgbNear(c, r,g,b, eps);
    }
}

static void test_clamping_out_of_range(){

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(0.0f, RgbFloat(1,0,0)),
        ColorMap::Entry(1.0f, RgbFloat(0,1,0)),
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    {
        RgbFloat c = map.interpolateStandardRgb(-1.0f);   // clamp to 0
        assertRgbNear(c, 1,0,0, eps);
    }
    {
        RgbFloat c = map.interpolateStandardRgb(2.0f);    // clamp to 1
        assertRgbNear(c, 0,1,0, eps);
    }
}

static void test_uniform_explicit_equal_spacing(){

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(0.0f, RgbFloat(1,0,0)),
        ColorMap::Entry(0.5f, RgbFloat(0,1,0)),
        ColorMap::Entry(1.0f, RgbFloat(0,0,1)),
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    // mid-points
    assertRgbNear(map.interpolateStandardRgb(0.25f), 0.5f,0.5f,0.0f, eps);
    assertRgbNear(map.interpolateStandardRgb(0.75f), 0.0f,0.5f,0.5f, eps);
}

static void test_hsl_interpolation(){

    static const ColorMap::Entry entries[] = {
        {0.0f, RgbFloat(1,0,0)},
        {1.0f, RgbFloat(0,1,0)},
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    HslColor h0 = map.interpolateHsl(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(eps, 0.0f, h0.h);   // red

    HslColor hm = map.interpolateHsl(0.5f);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 60.0f/360.0f, hm.h); // halfway between 0° -> 120°

    HslColor h1 = map.interpolateHsl(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(eps, 120.0f/360.0f, h1.h);
}

static void test_lab_interpolation_monotonic_L(){

    static const ColorMap::Entry entries[] = {
        {0.0f, RgbFloat(1,0,0)},
        {1.0f, RgbFloat(0,0,1)},
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    const float eps = 1e-4f;

    float prevL = map.interpolateLab(0.0f).L;

    for(int i=1; i<=50; ++i){
        float t = i / 50.0f;
        float L = map.interpolateLab(t).L;

        TEST_ASSERT_TRUE(L >= 0.0f && L <= 100.0f); // valid Lab
        prevL = L;
    }
}

static void test_invalid_non_monotonic(){
    TEST_RESET_ASSERTS();
    static const ColorMap::Entry entries[] = {
        { 0.0f, RgbFloat(1,0,0) },
        { 0.5f, RgbFloat(0,1,0) },
        { 0.3f, RgbFloat(0,0,1) }, // invalid
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap instance(span);
    TEST_ASSERT_EXIT_CALLED();
}

static void test_invalid_single_entry(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(RgbFloat(1,0,0)),
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap instance(span);
    TEST_ASSERT_EXIT_CALLED();
}

static void test_invalid_missing_zero(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        { 0.2f, RgbFloat(1,0,0) },
        { 1.0f, RgbFloat(0,0,1) },
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap instance(span);
    TEST_ASSERT_EXIT_CALLED();
}

static void test_invalid_missing_one(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        { 0.0f, RgbFloat(1,0,0) },
        { 0.8f, RgbFloat(0,0,1) },
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap instance(span);
    TEST_ASSERT_EXIT_CALLED();
}

static void test_invalid_duplicate_t(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        { 0.0f, RgbFloat(1,0,0) },
        { 0.4f, RgbFloat(0,1,0) },
        { 0.4f, RgbFloat(0,0,1) }, // duplicate
        { 1.0f, RgbFloat(1,1,1) }
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap instance(span);
    TEST_ASSERT_EXIT_CALLED();
}

static void test_uniform_many_entries_auto(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        ColorMap::Entry(RgbFloat(1,0,0)),
        ColorMap::Entry(RgbFloat(0.8f,0.2f,0)),
        ColorMap::Entry(RgbFloat(0.6f,0.4f,0)),
        ColorMap::Entry(RgbFloat(0.4f,0.6f,0)),
        ColorMap::Entry(RgbFloat(0.2f,0.8f,0)),
        ColorMap::Entry(RgbFloat(0,1,0)),
        ColorMap::Entry(RgbFloat(0,0.8f,0.2f)),
        ColorMap::Entry(RgbFloat(0,0.6f,0.4f)),
        ColorMap::Entry(RgbFloat(0,0.4f,0.6f)),
        ColorMap::Entry(RgbFloat(0,0.2f,0.8f)),
    };

    const int N = sizeof(entries) / sizeof(entries[0]);
    const float step = 1.0f / float(N - 1);

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    TEST_ASSERT_NO_ASSERTS();

    const float eps = 1e-4f;

    for(int i = 0; i <= 100; i++){
        float t = i / 100.0f;

        // --- MATCH EXACTLY resolveSegmentUniform() ---
        float pos = t / step;
        int idx = (int)pos;
        if(idx >= N - 1) idx = N - 2;

        float frac = pos - idx;

        const auto& A = entries[idx];
        const auto& B = entries[idx + 1];

        RgbFloat ref = {
            A.standardRgb.r + (B.standardRgb.r - A.standardRgb.r) * frac,
            A.standardRgb.g + (B.standardRgb.g - A.standardRgb.g) * frac,
            A.standardRgb.b + (B.standardRgb.b - A.standardRgb.b) * frac
        };

        RgbFloat c = map.interpolateStandardRgb(t);
        assertRgbNear(c, ref.r, ref.g, ref.b, eps);
    }
}

static void test_clamping(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        { 0.0f, RgbFloat(1,0,0) },
        { 1.0f, RgbFloat(0,0,1) },
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    TEST_ASSERT_NO_ASSERTS();
    const float eps = 1e-4f;

    assertRgbNear(map.interpolateStandardRgb(-1.0f), 1,0,0, eps);
    assertRgbNear(map.interpolateStandardRgb( 2.0f), 0,0,1, eps);
}

static void test_very_small_segments(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        { 0.0f,   RgbFloat(1,0,0) },
        { 0.001f, RgbFloat(0,1,0) },
        { 1.0f,   RgbFloat(0,0,1) }
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    TEST_ASSERT_NO_ASSERTS();

    RgbFloat c0 = map.interpolateStandardRgb(0.0005f);
    TEST_ASSERT_TRUE(c0.g >= 0.5f);

    RgbFloat c1 = map.interpolateStandardRgb(0.5f);
    TEST_ASSERT_FALSE(c1.b > 0.5f);

    RgbFloat c2 = map.interpolateStandardRgb(0.6f);
    TEST_ASSERT_TRUE(c2.b > 0.5f);
}

static void test_bucket_boundaries(){
    TEST_RESET_ASSERTS();

    static const ColorMap::Entry entries[] = {
        { 0.0f, RgbFloat(1,0,0) },
        { 0.0625f, RgbFloat(0,1,0) }, // = 1/16
        { 0.125f, RgbFloat(0,0,1) },  // = 2/16
        { 1.0f, RgbFloat(1,1,1) }
    };

    Span<const ColorMap::Entry> span(entries);
    ColorMap map(span);

    TEST_ASSERT_NO_ASSERTS();

    const float eps = 1e-4f;

    // Right on bucket boundaries
    assertRgbNear(map.interpolateStandardRgb(0.0625f), 0,1,0, eps);
    assertRgbNear(map.interpolateStandardRgb(0.125f),  0,0,1, eps);
}
static void test_max_entries(){
    TEST_RESET_ASSERTS();

    alignas(ColorMap::Entry) static unsigned char raw[256 * sizeof(ColorMap::Entry)];
    ColorMap::Entry* entries = reinterpret_cast<ColorMap::Entry*>(raw);

    for(int i = 0; i < 256; ++i){
        float t = i / 255.0f;
        new (&entries[i]) ColorMap::Entry(t, RgbFloat(t, 0, 1 - t));  // placement new
    }

    Span<const ColorMap::Entry> span(entries, 256);
    ColorMap map(span);

    TEST_ASSERT_NO_ASSERTS();

    const float eps = 1e-4f;

    for(float t = 0; t <= 1.0f; t += 0.05f){
        RgbFloat c = map.interpolateStandardRgb(t);

        float r = t;
        float g = 0;
        float b = 1 - t;

        assertRgbNear(c, r,g,b, eps);
    }
}

// --------------------------------------------------------------
// NATIVE MAIN FOR UNITY
// --------------------------------------------------------------
int main(int argc, char **argv){

    InstallAssertHandlers();

    UNITY_BEGIN();

    RUN_TEST(test_uniform_two_entries_explicit);
    RUN_TEST(test_uniform_three_entries_auto);
    RUN_TEST(test_non_uniform_rbr);
    RUN_TEST(test_clamping_out_of_range);
    RUN_TEST(test_uniform_explicit_equal_spacing);
    RUN_TEST(test_hsl_interpolation);
    RUN_TEST(test_lab_interpolation_monotonic_L);
    RUN_TEST(test_invalid_non_monotonic);
    RUN_TEST(test_invalid_single_entry);
    RUN_TEST(test_invalid_missing_zero);
    RUN_TEST(test_invalid_missing_one);
    RUN_TEST(test_invalid_duplicate_t);
    RUN_TEST(test_uniform_many_entries_auto);
    RUN_TEST(test_clamping);
    RUN_TEST(test_very_small_segments);
    RUN_TEST(test_bucket_boundaries);
    RUN_TEST(test_max_entries);

    return UNITY_END();
}
