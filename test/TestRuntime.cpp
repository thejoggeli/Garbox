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

class ExhaustionRuntime final : public RuntimeAbs {
public:
    ExhaustionRuntime(): RuntimeAbs(makeConfig()){}

    using RuntimeAbs::dispatch;

    const EventHeader* submitHeartbeat(){
        auto event = getEventFactory().make<EventType::Heartbeat>(
            {ComponentType::Runtime, ComponentId::Runtime});
        receiveEvent(event.header());
        return event.header();
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
    }

    void onActiveBehaviourChanged() override {}
    void onActiveScreenChanged() override {}

    BehaviourAbs* resolveBehaviour(BehaviourId) override { return nullptr; }
    ControllerAbs* resolveController(ControllerId) override { return nullptr; }
    ScreenAbs* resolveScreen(ScreenId) override { return nullptr; }

private:
    static Config makeConfig(){
        Config config;
        config.numComponents = 0;
        config.numStates = 0;
        config.eventPoolSizeBytes = 256;
        config.eventQueueLength = 1;
        config.maxDispatchRecursionDepth = 1;
        return config;
    }

    std::vector<uint32_t> mRoutedIds;
};

class PayloadRuntime final : public RuntimeAbs {
public:
    PayloadRuntime(): RuntimeAbs(makeConfig()){}

    using RuntimeAbs::dispatch;

    DisplayCommandEvent makeDisplayCommand(){
        return getEventFactory().make<EventType::DisplayCommand>(
            {ComponentType::Runtime, ComponentId::Runtime});
    }

    void submit(const DisplayCommandEvent& event){
        receiveEvent(event.header());
    }

    float routedBrightness() const { return mRoutedBrightness; }

protected:
    void onInit() override {}
    void onStart() override {}
    void onRun(const TaskStopToken&) override {}
    void onRouteStateChanged(const StateAbs&) override {}
    void onRouteEvent(const EventHeader* header) override {
        const DisplayCommandEvent event(header);
        mRoutedBrightness = event->brightness;
    }
    void onActiveBehaviourChanged() override {}
    void onActiveScreenChanged() override {}
    BehaviourAbs* resolveBehaviour(BehaviourId) override { return nullptr; }
    ControllerAbs* resolveController(ControllerId) override { return nullptr; }
    ScreenAbs* resolveScreen(ScreenId) override { return nullptr; }

private:
    static Config makeConfig(){
        Config config;
        config.numComponents = 0;
        config.numStates = 0;
        config.eventPoolSizeBytes = 256;
        config.eventQueueLength = 4;
        config.maxDispatchRecursionDepth = 2;
        return config;
    }

    float mRoutedBrightness = -1.0f;
};

class TestBehaviour final : public BehaviourAbs {
public:
    TestBehaviour(ComponentId componentId, BehaviourId behaviourId):
        BehaviourAbs(componentId, behaviourId){}
};

class TestScreen final : public ScreenAbs {
public:
    TestScreen(ComponentId componentId, ScreenId screenId):
        ScreenAbs(componentId, screenId){}
protected:
    void onRender() override {}
};

class ModeRuntime final : public RuntimeAbs {
public:
    ModeRuntime():
        RuntimeAbs(makeConfig()),
        mCalibration(ComponentId::CalibrationBehaviour, BehaviourId::Calibration),
        mFermentation(ComponentId::FermentationBehaviour, BehaviourId::Fermentation),
        mSimple(ComponentId::SimpleScreen, ScreenId::Simple),
        mMain(ComponentId::MainScreen, ScreenId::Main){
        registerComponent(&mCalibration);
        registerComponent(&mFermentation);
        registerComponent(&mSimple);
        registerComponent(&mMain);
    }

    void applyBehaviour(){
        applyQueuedBehaviour();
    }
    bool hasQueuedBehaviour() const { return mQueuedBehaviour != nullptr; }
    bool hasQueuedScreen() const { return mQueuedScreen != nullptr; }

protected:
    void onInit() override {}
    void onStart() override {}
    void onRun(const TaskStopToken&) override {}
    void onRouteStateChanged(const StateAbs&) override {}
    void onRouteEvent(const EventHeader*) override {}
    void onActiveBehaviourChanged() override { ++mBehaviourChanges; }
    void onActiveScreenChanged() override { ++mScreenChanges; }
    BehaviourAbs* resolveBehaviour(BehaviourId id) override {
        if(id == BehaviourId::Calibration) return &mCalibration;
        if(id == BehaviourId::Fermentation) return &mFermentation;
        return nullptr;
    }
    ControllerAbs* resolveController(ControllerId) override { return nullptr; }
    ScreenAbs* resolveScreen(ScreenId id) override {
        if(id == ScreenId::Simple) return &mSimple;
        if(id == ScreenId::Main) return &mMain;
        return nullptr;
    }

private:
    static Config makeConfig(){
        Config config;
        config.numComponents = 4;
        config.numStates = 0;
        config.eventPoolSizeBytes = 512;
        config.eventQueueLength = 8;
        config.maxDispatchRecursionDepth = 2;
        return config;
    }

    TestBehaviour mCalibration;
    TestBehaviour mFermentation;
    TestScreen mSimple;
    TestScreen mMain;
    uint32_t mBehaviourChanges = 0;
    uint32_t mScreenChanges = 0;
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

void test_event_pool_exhaustion_returns_safe_invalid_view(){
    TEST_RESET_ASSERTS();
    static EventFactory* factory = new EventFactory(0);

    auto event = factory->make<EventType::DisplayCommand>(
        {ComponentType::Runtime, ComponentId::Runtime});

    TEST_ASSERT_FALSE(event.valid());
    TEST_ASSERT_FALSE(static_cast<bool>(event));
    TEST_ASSERT_NULL(event.header());
    TEST_ASSERT_NOT_NULL(event.payload());

    // A returning host fatal handler must not turn normal payload setup into a
    // null dereference. This write is intentionally discarded.
    event->brightness = 0.75f;

    TEST_ASSERT_EQUAL_UINT32(1U, AssertDebugCount);
    TEST_ASSERT_EQUAL_UINT32(1U, AssertExitCount);
}

void test_event_queue_exhaustion_rejects_new_event_and_preserves_queue(){
    TEST_RESET_ASSERTS();
    static ExhaustionRuntime* runtime = new ExhaustionRuntime();

    const EventHeader* accepted = runtime->submitHeartbeat();
    const EventHeader* rejected = runtime->submitHeartbeat();

    TEST_ASSERT_NOT_NULL(accepted);
    TEST_ASSERT_NOT_NULL(rejected);
    TEST_ASSERT_EQUAL_UINT32(1U, runtime->getContext().eventCount);
    TEST_ASSERT_EQUAL_UINT32(1U, AssertExitCount);

    TEST_RESET_ASSERTS();
    runtime->dispatch();

    TEST_ASSERT_EQUAL_UINT32(1U, runtime->routedIds().size());
    TEST_ASSERT_EQUAL_UINT32(1U, runtime->routedIds()[0]);
    TEST_ASSERT_NO_ASSERTS();
}

void test_runtime_rejects_null_event_without_counting_or_routing(){
    TEST_RESET_ASSERTS();
    static ExhaustionRuntime* runtime = new ExhaustionRuntime();
    const uint32_t countBefore = runtime->getContext().eventCount;

    runtime->receiveEvent(nullptr);

    TEST_ASSERT_EQUAL_UINT32(countBefore, runtime->getContext().eventCount);
    TEST_ASSERT_TRUE(runtime->routedIds().empty());
    TEST_ASSERT_EQUAL_UINT32(1U, AssertExitCount);
}

void test_submitted_event_is_immutable_and_cannot_be_submitted_twice(){
    TEST_RESET_ASSERTS();
    static PayloadRuntime* runtime = new PayloadRuntime();
    DisplayCommandEvent event = runtime->makeDisplayCommand();
    event->brightness = 0.25f;
    runtime->submit(event);

    // The second write targets discard storage and cannot alter the queued
    // payload. A second submission is rejected as a separate contract error.
    event->brightness = 0.75f;
    runtime->submit(event);
    TEST_ASSERT_EQUAL_UINT32(2U, AssertExitCount);

    TEST_RESET_ASSERTS();
    runtime->dispatch();
    TEST_ASSERT_EQUAL_FLOAT(0.25f, runtime->routedBrightness());
    TEST_ASSERT_NO_ASSERTS();
}

void test_redundant_mode_requests_clear_or_cancel_pending_switches(){
    TEST_RESET_ASSERTS();
    static ModeRuntime* runtime = new ModeRuntime();
    runtime->init();

    runtime->requestChangeBehaviour(BehaviourId::Calibration);
    runtime->requestChangeScreen(ScreenId::Simple);
    TEST_ASSERT_EQUAL(BehaviourId::Calibration, runtime->getActiveBehaviourId());
    TEST_ASSERT_EQUAL(ScreenId::Simple, runtime->getActiveScreenId());

    runtime->requestChangeBehaviour(BehaviourId::Fermentation);
    runtime->requestChangeScreen(ScreenId::Main);
    TEST_ASSERT_TRUE(runtime->hasQueuedBehaviour());
    TEST_ASSERT_TRUE(runtime->hasQueuedScreen());

    // The newest request asks to remain in the current mode, so it cancels the
    // older pending switch instead of being retried on every tick forever.
    runtime->requestChangeBehaviour(BehaviourId::Calibration);
    runtime->requestChangeScreen(ScreenId::Simple);
    TEST_ASSERT_FALSE(runtime->hasQueuedBehaviour());
    TEST_ASSERT_FALSE(runtime->hasQueuedScreen());
    runtime->applyBehaviour();
    TEST_ASSERT_EQUAL(BehaviourId::Calibration, runtime->getActiveBehaviourId());
    TEST_ASSERT_EQUAL(ScreenId::Simple, runtime->getActiveScreenId());
    TEST_ASSERT_EQUAL_UINT32(2U, AssertDebugCount);
    TEST_ASSERT_EQUAL_UINT32(0U, AssertExitCount);
}

} // namespace

void RegisterRuntimeTests(){
    RUN_TEST(test_event_cascade_at_pass_limit_releases_pool_when_stable);
    RUN_TEST(test_event_cascade_beyond_pass_limit_retains_queued_storage);
    RUN_TEST(test_event_pool_exhaustion_returns_safe_invalid_view);
    RUN_TEST(test_event_queue_exhaustion_rejects_new_event_and_preserves_queue);
    RUN_TEST(test_runtime_rejects_null_event_without_counting_or_routing);
    RUN_TEST(test_submitted_event_is_immutable_and_cannot_be_submitted_twice);
    RUN_TEST(test_redundant_mode_requests_clear_or_cancel_pending_switches);
}
