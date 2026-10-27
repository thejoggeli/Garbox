#include <unity.h>

#include "core/application/runtime/RuntimeAbs.h"

// The runtime is the production owner of publish(). This visibility override
// lets the host test exercise the generated transition without widening the
// firmware API solely for testing.
#define private public
#include "generated/states/types/DisplayStatusState.h"
#undef private

using namespace Garbox;

namespace {

class StateTestBehaviour final : public BehaviourAbs {
public:
    StateTestBehaviour():
        BehaviourAbs(ComponentId::CalibrationBehaviour, BehaviourId::Calibration){}
};

class StateTestRuntime final : public RuntimeAbs {
public:
    StateTestRuntime(): RuntimeAbs(makeConfig()){
        registerComponent(&mBehaviour);
        registerState(&mState);
    }

    using RuntimeAbs::dispatch;
    DisplayStatusState& state(){ return mState; }
    uint32_t callbackCount() const { return mCallbackCount; }
    float firstCallbackCurrent() const { return mFirstCallbackCurrent; }
    float firstCallbackPending() const { return mFirstCallbackPending; }
    void cascadeOnNextCallback(float value){
        mCascade = true;
        mCascadeValue = value;
    }

protected:
    void onInit() override {}
    void onStart() override {}
    void onRun(const TaskStopToken&) override {}
    void onRouteStateChanged(const StateAbs& state) override {
        TEST_ASSERT_EQUAL_UINT16(
            static_cast<uint16_t>(StateType::DisplayStatus),
            static_cast<uint16_t>(state.type()));
        ++mCallbackCount;
        if(mCallbackCount == 1){
            mFirstCallbackCurrent = mState.getBrightness();
            mFirstCallbackPending = mState.nextBrightness();
        }
        if(mCascade){
            mCascade = false;
            mState.setBrightness(mCascadeValue);
        }
    }
    void onRouteEvent(const EventHeader*) override {}
    void onActiveBehaviourChanged() override {}
    void onActiveScreenChanged() override {}
    BehaviourAbs* resolveBehaviour(BehaviourId) override { return nullptr; }
    ControllerAbs* resolveController(ControllerId) override { return nullptr; }
    ScreenAbs* resolveScreen(ScreenId) override { return nullptr; }

private:
    static Config makeConfig(){
        Config config;
        config.numComponents = 1;
        config.numStates = 1;
        config.eventPoolSizeBytes = 64;
        config.eventQueueLength = 2;
        config.maxDispatchRecursionDepth = 3;
        return config;
    }

    StateTestBehaviour mBehaviour;
    DisplayStatusState mState;
    uint32_t mCallbackCount = 0;
    float mFirstCallbackCurrent = -1.0f;
    float mFirstCallbackPending = -1.0f;
    bool mCascade = false;
    float mCascadeValue = 0.0f;
};

void test_generated_state_separates_pending_current_and_last(){
    DisplayStatusState state;

    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.lastBrightness());

    state.setBrightness(0.25f);

    TEST_ASSERT_TRUE(state.isDirty());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.lastBrightness());

    state.publish();

    TEST_ASSERT_FALSE(state.isDirty());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, state.lastBrightness());

    state.setBrightness(0.75f);
    state.publish();

    TEST_ASSERT_EQUAL_FLOAT(0.75f, state.getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.75f, state.nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, state.lastBrightness());
}

void test_runtime_coalesces_state_writes_and_publishes_callback_changes_next(){
    static StateTestRuntime* runtime = new StateTestRuntime();
    runtime->init();

    runtime->state().setBrightness(0.10f);
    runtime->state().setBrightness(0.20f);
    runtime->state().setBrightness(0.25f);
    runtime->cascadeOnNextCallback(0.75f);
    runtime->dispatch();

    // Three writes before the visibility boundary coalesce to one callback.
    // The callback-generated update is then published in the next dispatch
    // pass, producing the second callback without losing last/current data.
    TEST_ASSERT_EQUAL_UINT32(2U, runtime->callbackCount());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, runtime->firstCallbackCurrent());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, runtime->firstCallbackPending());
    TEST_ASSERT_EQUAL_FLOAT(0.75f, runtime->state().getBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.75f, runtime->state().nextBrightness());
    TEST_ASSERT_EQUAL_FLOAT(0.25f, runtime->state().lastBrightness());
    TEST_ASSERT_FALSE(runtime->state().isDirty());
}

} // namespace

void RegisterStateTests(){
    RUN_TEST(test_generated_state_separates_pending_current_and_last);
    RUN_TEST(test_runtime_coalesces_state_writes_and_publishes_callback_changes_next);
}
