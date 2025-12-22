// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include "core/lvgl/LvglProvider.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs(): 
    ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug),
    mRoot(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()),
    mRenderDispatcher(static_cast<uint32_t>(RenderFn::Count)){}

DebugScreenAbs::States& DebugScreenAbs::states(){
    return mStates.value();
}

void DebugScreenAbs::bindStates(
    const FanStatusState& fanStatus,
    const FanSampleState& fanSample,
    const HeatpadStatusState& heatpadStatus,
    const HeatpadSampleState& heatpadSample,
    const HeatpadProgressState& heatpadProgress,
    const DisplayStatusState& displayStatus,
    const DisplayDiagnosticsState& displayDiagnostics,
    const TemperatureStatusState& temperatureStatus,
    const TemperatureSampleState& temperatureSample,
    const ActiveBehaviourState& activeBehaviour,
    const ActiveScreenState& activeScreen,
    const FermentationStatusState& fermentationStatus
){
    mStates.emplace(
        fanStatus,
        fanSample,
        heatpadStatus,
        heatpadSample,
        heatpadProgress,
        displayStatus,
        displayDiagnostics,
        temperatureStatus,
        temperatureSample,
        activeBehaviour,
        activeScreen,
        fermentationStatus
    );
}

void DebugScreenAbs::init(ComponentHostIfc& host){

    // init lvgl container
    mRoot.setRawSize(mScreenWidth, mScreenHeight);
    mRoot.setBorder(0, lv_color_hex(0x000000));
    mRoot.setRadius(0);
    mRoot.setPad(0, 0, 0, 0);
    mRoot.setBgOpa(LV_OPA_COVER);
    mRoot.setScrollable(false);

    mRenderDispatcher.registerHandler(renderFanStateTrampoline, this);
    mRenderDispatcher.registerHandler(renderFanMeasuredRpmTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeatpadStateTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeatpadDutyTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeatpadSenseTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeatpadProgressTrampoline, this);
    mRenderDispatcher.registerHandler(renderDisplayStatusTrampoline, this);
    mRenderDispatcher.registerHandler(renderTemperatureStateTrampoline, this);
    mRenderDispatcher.registerHandler(renderTemperatureSampleTrampoline, this);
    mRenderDispatcher.registerHandler(renderTimeTrampoline, this);
    mRenderDispatcher.registerHandler(renderAppInfoTrampoline, this);
    mRenderDispatcher.registerHandler(renderFermentationStatusTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeapBlocksTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeapBytesTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeapMinimumTrampoline, this);

    // calls onInit()
    ScreenAbs::init(host);
}

void DebugScreenAbs::render(){
    ScreenAbs::render();
    mDispatchedCount += mRenderDispatcher.getDirtyCount();
    mRenderDispatcher.dispatch();
}

void DebugScreenAbs::becomeEnabled(){
    mRoot.setScreen();

    // mark all render functions dirty for complete redraw 
    markDirty(RenderFn::FanState);
    markDirty(RenderFn::FanMeasuredRpm);
    markDirty(RenderFn::HeatpadState);
    markDirty(RenderFn::HeatpadDuty);
    markDirty(RenderFn::HeatpadSense);
    markDirty(RenderFn::HeatpadProgress);
    markDirty(RenderFn::DisplayStatus);
    markDirty(RenderFn::TemperatureState);
    markDirty(RenderFn::TemperatureSample);
    markDirty(RenderFn::Time);
    markDirty(RenderFn::AppInfo);
    markDirty(RenderFn::FermentationStatus);
    markDirty(RenderFn::HeapBlocks);
    markDirty(RenderFn::HeapBytes);
    markDirty(RenderFn::HeapMinimum);

    ScreenAbs::becomeEnabled();
}

void DebugScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

void DebugScreenAbs::setBackgroundColor(uint32_t color){
    mRoot.setBgColor(lv_color_hex(color));
}

void DebugScreenAbs::markDirty(RenderFn fn){
    mRenderDispatcher.markDirty(static_cast<size_t>(fn));
}

bool DebugScreenAbs::isMarkedDirty(RenderFn fn) const {
    return mRenderDispatcher.isMarkedDirty(static_cast<size_t>(fn));
}

void DebugScreenAbs::renderFanStateTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderFanState();
}

void DebugScreenAbs::renderFanMeasuredRpmTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderFanMeasuredRpm();
}

void DebugScreenAbs::renderHeatpadStateTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeatpadState();
}

void DebugScreenAbs::renderHeatpadDutyTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeatpadDuty();
}

void DebugScreenAbs::renderHeatpadSenseTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeatpadSense();
}

void DebugScreenAbs::renderHeatpadProgressTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeatpadProgress();
}

void DebugScreenAbs::renderDisplayStatusTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderDisplayStatus();
}

void DebugScreenAbs::renderTemperatureStateTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderTemperatureState();
}

void DebugScreenAbs::renderTemperatureSampleTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderTemperatureSample();
}

void DebugScreenAbs::renderTimeTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderTime();
}

void DebugScreenAbs::renderAppInfoTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderAppInfo();
}

void DebugScreenAbs::renderFermentationStatusTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderFermentationStatus();
}

void DebugScreenAbs::renderHeapBlocksTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeapBlocks();
}

void DebugScreenAbs::renderHeapBytesTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeapBytes();
}

void DebugScreenAbs::renderHeapMinimumTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onRenderHeapMinimum();
}

} // namespace Garbox