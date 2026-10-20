#include <cstddef>
#include <cstdint>

#include <unity.h>

#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/static/BitsetStatic.h"
#include "core/util/container/static/DataPoolStatic.h"
#include "core/util/container/static/RingBufferStatic.h"
#include "core/util/container/static/VectorStatic.h"
#include "core/util/container/storage/StaticStorage.h"

using namespace Garbox;

namespace {

void test_static_storage_reports_capacity_for_element_type(){
    StaticStorage<uint8_t, 7> bytes;
    StaticStorage<uint16_t, 7> words;
    StaticStorage<uint32_t, 7> dwords;

    TEST_ASSERT_EQUAL_UINT32(7, bytes.capacityElements());
    TEST_ASSERT_EQUAL_UINT32(7 * sizeof(uint8_t), bytes.capacityBytes());
    TEST_ASSERT_EQUAL_UINT32(7 * sizeof(uint16_t), words.capacityBytes());
    TEST_ASSERT_EQUAL_UINT32(7 * sizeof(uint32_t), dwords.capacityBytes());
}

void test_static_byte_pool_uses_exact_declared_capacity(){
    DataPoolStatic<7> pool;

    TEST_ASSERT_EQUAL_UINT32(7, pool.capacity());
    TEST_ASSERT_NOT_NULL(pool.allocateRaw(7, alignof(uint8_t)));
    TEST_ASSERT_EQUAL_UINT32(7, pool.used());
    TEST_ASSERT_NULL(pool.allocateRaw(1, alignof(uint8_t)));
}

void test_representative_static_container_headers_are_self_contained(){
    ArrayStatic<uint16_t, 3> array;
    BitsetStatic<9> bits;
    RingBufferStatic<uint16_t, 3> ring;
    VectorStatic<uint16_t, 3> vector;

    TEST_ASSERT_EQUAL_UINT32(3, array.capacity());
    TEST_ASSERT_EQUAL_UINT32(9, bits.size());
    TEST_ASSERT_EQUAL_UINT32(3, ring.capacity());
    TEST_ASSERT_EQUAL_UINT32(3, vector.capacity());
}

} // namespace

void RegisterContainerTests(){
    RUN_TEST(test_static_storage_reports_capacity_for_element_type);
    RUN_TEST(test_static_byte_pool_uses_exact_declared_capacity);
    RUN_TEST(test_representative_static_container_headers_are_self_contained);
}
