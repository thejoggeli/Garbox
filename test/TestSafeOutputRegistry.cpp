#include <cstdint>

#include <unity.h>

#include "core/safety/SafeOutputRegistry.h"

using namespace Garbox;

namespace {

struct Probe {
    uint32_t calls = 0U;
    uint32_t order = 0U;
    uint32_t* nextOrder = nullptr;
};

void handleProbe(void* context){
    Probe& probe = *static_cast<Probe*>(context);
    ++probe.calls;
    probe.order = ++(*probe.nextOrder);
}

void test_safe_outputs_apply_once_in_registration_order(){
    SafeOutputRegistry<2U> registry;
    uint32_t nextOrder = 0U;
    Probe first{0U, 0U, &nextOrder};
    Probe second{0U, 0U, &nextOrder};

    TEST_ASSERT_TRUE(registry.registerHandler(handleProbe, &first));
    TEST_ASSERT_TRUE(registry.registerHandler(handleProbe, &second));
    TEST_ASSERT_EQUAL_UINT32(2U, registry.size());

    TEST_ASSERT_TRUE(registry.apply());
    TEST_ASSERT_FALSE(registry.apply());
    TEST_ASSERT_TRUE(registry.applied());
    TEST_ASSERT_EQUAL_UINT32(1U, first.calls);
    TEST_ASSERT_EQUAL_UINT32(1U, second.calls);
    TEST_ASSERT_EQUAL_UINT32(1U, first.order);
    TEST_ASSERT_EQUAL_UINT32(2U, second.order);
}

void test_safe_output_registry_rejects_invalid_late_and_excess_handlers(){
    SafeOutputRegistry<1U> registry;
    uint32_t nextOrder = 0U;
    Probe probe{0U, 0U, &nextOrder};

    TEST_ASSERT_FALSE(registry.registerHandler(nullptr, &probe));
    TEST_ASSERT_TRUE(registry.registerHandler(handleProbe, &probe));
    TEST_ASSERT_FALSE(registry.registerHandler(handleProbe, &probe));
    TEST_ASSERT_TRUE(registry.apply());
    TEST_ASSERT_FALSE(registry.registerHandler(handleProbe, &probe));
}

} // namespace

void RegisterSafeOutputRegistryTests(){
    RUN_TEST(test_safe_outputs_apply_once_in_registration_order);
    RUN_TEST(test_safe_output_registry_rejects_invalid_late_and_excess_handlers);
}
