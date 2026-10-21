#include <cstddef>
#include <cstdint>
#include <vector>

#include <unity.h>

#include "core/application/runtime/RuntimeAbs.h"
#include "helpers/AssertHelper.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

class CascadeRuntime final : public RuntimeAbs {
public:
    explicit CascadeRuntime(std::size_t maxDispatchPasses):
        RuntimeAbs(makeConfig(maxDispatchPasses)){}

    using RuntimeAbs::dispatch;

    const EventHeader* submitHeartbeat(){
        auto event = getEventFactory().make<EventType::Heartbeat>(
            {ComponentType::Runtime, ComponentId::Runtime});
        if(event.header()){
            receiveEvent(event.header());
        }
        return event.header();
    }

    void cascadeFor(std::size_t count){
        mCascadeRemaining = count;
    }

    const std::vector<uint32_t>& routedIds() const {
        return mRoutedIds;
    }

protected:
    void onInit() override {}
    void onStart() override {}
    void onRun(const TaskStopToken&) override {}
    void onRouteStateChanged(const StateAbs&) override {}

    void onRouteEvent(const EventHeader* event) override {
        mRoutedIds.push_back(event->id);
        if(mCascadeRemaining > 0){
            --mCascadeRemaining;
            submitHeartbeat();
        }
    }

    void onActiveBehaviourChanged() override {}
    void onActiveScreenChanged() override {}

    BehaviourAbs* resolveBehaviour(BehaviourId) override { return nullptr; }
    ControllerAbs* resolveController(ControllerId) override { return nullptr; }
    ScreenAbs* resolveScreen(ScreenId) override { return nullptr; }

private:
    static Config makeConfig(std::size_t maxDispatchPasses){
        Config config;
        config.numComponents = 0;
        config.numStates = 0;
        config.eventPoolSizeBytes = 1024;
        config.eventQueueLength = 16;
        config.maxDispatchRecursionDepth = maxDispatchPasses;
        return config;
    }

    std::size_t mCascadeRemaining = 0;
    std::vector<uint32_t> mRoutedIds;
};

void assertRoutedIds(const CascadeRuntime& runtime,
                     const std::vector<uint32_t>& expected){
    TEST_ASSERT_EQUAL_UINT32(expected.size(), runtime.routedIds().size());
    for(std::size_t i = 0; i < expected.size(); ++i){
        TEST_ASSERT_EQUAL_UINT32(expected[i], runtime.routedIds()[i]);
    }
}

void test_event_cascade_at_pass_limit_releases_pool_when_stable(){
    TEST_RESET_ASSERTS();
    static CascadeRuntime* runtime = new CascadeRuntime(2);
    runtime->cascadeFor(1);

    const EventHeader* firstAllocation = runtime->submitHeartbeat();
    TEST_ASSERT_NOT_NULL(firstAllocation);
    runtime->dispatch();

    assertRoutedIds(*runtime, {1, 2});
    TEST_ASSERT_NO_ASSERTS();

    // Stable completion resets the bump pool, so its first address is reused.
    const EventHeader* allocationAfterStableDispatch = runtime->submitHeartbeat();
    TEST_ASSERT_EQUAL_PTR(firstAllocation, allocationAfterStableDispatch);
}

void test_event_cascade_beyond_pass_limit_retains_queued_storage(){
    TEST_RESET_ASSERTS();
    static CascadeRuntime* runtime = new CascadeRuntime(2);
    runtime->cascadeFor(2);

    const EventHeader* firstAllocation = runtime->submitHeartbeat();
    TEST_ASSERT_NOT_NULL(firstAllocation);
    runtime->dispatch();

    // IDs 1 and 2 route in the two allowed passes; ID 3 stays queued.
    assertRoutedIds(*runtime, {1, 2});
    TEST_ASSERT_EQUAL_UINT32(0U, AssertDebugCount);
    TEST_ASSERT_EQUAL_UINT32(1U, AssertExitCount);

    // These allocations overwrite ID 3 under the old premature pool reset.
    TEST_RESET_ASSERTS();
    runtime->submitHeartbeat();
    runtime->submitHeartbeat();
    runtime->submitHeartbeat();
    runtime->dispatch();

    assertRoutedIds(*runtime, {1, 2, 3, 4, 5, 6});
    TEST_ASSERT_NO_ASSERTS();

    // The second dispatch is stable, so the pool may now be reused safely.
    const EventHeader* allocationAfterStableDispatch = runtime->submitHeartbeat();
    TEST_ASSERT_EQUAL_PTR(firstAllocation, allocationAfterStableDispatch);
}

} // namespace

void RegisterRuntimeTests(){
    RUN_TEST(test_event_cascade_at_pass_limit_releases_pool_when_stable);
    RUN_TEST(test_event_cascade_beyond_pass_limit_retains_queued_storage);
}
