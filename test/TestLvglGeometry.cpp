#include <unity.h>

#include "core/lvgl/helpers/Rgb565DrawBufferGeometry.h"

using namespace Garbox;

namespace {

void test_rgb565_partial_buffer_geometry(){
    constexpr uint32_t width = 320U;
    constexpr uint32_t rows = 60U;

    static_assert(Rgb565DrawBufferGeometry::BytesPerPixel == 2U);
    static_assert(Rgb565DrawBufferGeometry::strideBytes(width) == 640U);
    static_assert(Rgb565DrawBufferGeometry::sizeBytes(width, rows) == 38'400U);

    TEST_ASSERT_EQUAL_UINT32(640U, Rgb565DrawBufferGeometry::strideBytes(width));
    TEST_ASSERT_EQUAL_UINT32(38'400U, Rgb565DrawBufferGeometry::sizeBytes(width, rows));
}

} // namespace

void RegisterLvglGeometryTests(){
    RUN_TEST(test_rgb565_partial_buffer_geometry);
}
