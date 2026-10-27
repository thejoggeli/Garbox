#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <unity.h>

#include "core/util/container/heap/ArrayHeap.h"
#include "core/util/container/heap/VectorHeap.h"
#include "core/util/container/static/MaskedArrayStatic.h"
#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/static/BitsetStatic.h"
#include "core/util/container/static/DataPoolStatic.h"
#include "core/util/container/static/RingBufferStatic.h"
#include "core/util/container/static/VectorStatic.h"
#include "core/util/container/storage/StaticStorage.h"
#include "helpers/AssertHelper.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

struct LifetimeProbe {
    static int live;
    static int destroyed;

    explicit LifetimeProbe(int value = 0): value(value){ ++live; }
    LifetimeProbe(const LifetimeProbe& other): value(other.value){ ++live; }
    ~LifetimeProbe(){
        --live;
        ++destroyed;
    }

    int value;
};

int LifetimeProbe::live = 0;
int LifetimeProbe::destroyed = 0;

void resetLifetimeProbe(){
    LifetimeProbe::live = 0;
    LifetimeProbe::destroyed = 0;
}

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

void test_arrays_and_masked_arrays_destroy_every_constructed_element(){
    resetLifetimeProbe();
    {
        ArrayHeap<LifetimeProbe> array(3);
        TEST_ASSERT_EQUAL_INT(3, LifetimeProbe::live);
    }
    TEST_ASSERT_EQUAL_INT(0, LifetimeProbe::live);
    TEST_ASSERT_EQUAL_INT(3, LifetimeProbe::destroyed);

    resetLifetimeProbe();
    {
        MaskedArrayStatic<LifetimeProbe, 4> array(false);
        array.setIndexEnabled(2, true);
        TEST_ASSERT_EQUAL_INT(4, LifetimeProbe::live);
    }
    TEST_ASSERT_EQUAL_INT(0, LifetimeProbe::live);
    TEST_ASSERT_EQUAL_INT(4, LifetimeProbe::destroyed);
}

void test_vector_and_ring_destroy_only_live_elements(){
    resetLifetimeProbe();
    {
        VectorHeap<LifetimeProbe> vector(3);
        vector.emplace(1);
        vector.emplace(2);
        TEST_ASSERT_EQUAL_INT(2, LifetimeProbe::live);
        TEST_ASSERT_TRUE(vector.destroyLast());
        TEST_ASSERT_EQUAL_INT(1, LifetimeProbe::live);
    }
    TEST_ASSERT_EQUAL_INT(0, LifetimeProbe::live);
    TEST_ASSERT_EQUAL_INT(2, LifetimeProbe::destroyed);

    resetLifetimeProbe();
    {
        RingBufferStatic<LifetimeProbe, 3> ring;
        ring.emplaceBack(1);
        ring.emplaceFront(2);
        TEST_ASSERT_EQUAL_INT(2, LifetimeProbe::live);
        TEST_ASSERT_TRUE(ring.destroyBack());
        TEST_ASSERT_EQUAL_INT(1, LifetimeProbe::live);
    }
    TEST_ASSERT_EQUAL_INT(0, LifetimeProbe::live);
    TEST_ASSERT_EQUAL_INT(2, LifetimeProbe::destroyed);
}

void test_heap_containers_are_raii_owned_and_not_copyable(){
    static_assert(!std::is_copy_constructible<ArrayHeap<uint32_t>>::value, "heap arrays must not alias ownership");
    static_assert(!std::is_move_constructible<ArrayHeap<uint32_t>>::value, "heap arrays do not transfer stable storage");
    static_assert(!std::is_copy_constructible<ArrayStatic<uint32_t, 2>>::value, "raw object storage must not be copied");
    static_assert(!std::is_copy_constructible<VectorStatic<uint32_t, 2>>::value, "vector lifetime state must not be copied");
    static_assert(!std::is_copy_constructible<RingBufferStatic<uint32_t, 2>>::value, "ring lifetime state must not be copied");

    ArrayHeap<std::max_align_t> array(2);
    TEST_ASSERT_NOT_NULL(array.data());
    TEST_ASSERT_EQUAL_UINT64(
        0,
        reinterpret_cast<std::uintptr_t>(array.data()) % alignof(std::max_align_t)
    );
}

void test_pool_rejects_bad_alignment_without_advancing(){
    TEST_RESET_ASSERTS();
    DataPoolStatic<16> pool;
    TEST_ASSERT_NULL(pool.allocateRaw(1, 0));
    TEST_ASSERT_NULL(pool.allocateRaw(1, 3));
    TEST_ASSERT_EQUAL_UINT32(0, pool.used());
    TEST_ASSERT_EQUAL_UINT32(2, AssertDebugCount);
}

} // namespace

void RegisterContainerTests(){
    RUN_TEST(test_static_storage_reports_capacity_for_element_type);
    RUN_TEST(test_static_byte_pool_uses_exact_declared_capacity);
    RUN_TEST(test_representative_static_container_headers_are_self_contained);
    RUN_TEST(test_arrays_and_masked_arrays_destroy_every_constructed_element);
    RUN_TEST(test_vector_and_ring_destroy_only_live_elements);
    RUN_TEST(test_heap_containers_are_raii_owned_and_not_copyable);
    RUN_TEST(test_pool_rejects_bad_alignment_without_advancing);
}
