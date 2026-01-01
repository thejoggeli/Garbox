// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "SimpleScreenAbs.h"
#include "core/lvgl/LvglProvider.h"

namespace Garbox {

SimpleScreenAbs::SimpleScreenAbs(): 
    ScreenAbs(ComponentId::SimpleScreen, ScreenId::Simple),
    mGui(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()),
    mRenderDispatcher(static_cast<uint32_t>(RenderFn::Count)){}

RequestFermentationModeEvent SimpleScreenAbs::makeRequestFermentationModeEvent(){
    return ComponentAbs::makeEvent<EventType::RequestFermentationMode>();
}

RequestTargetTemperatureEvent SimpleScreenAbs::makeRequestTargetTemperatureEvent(){
    return ComponentAbs::makeEvent<EventType::RequestTargetTemperature>();
}

void SimpleScreenAbs::sendEvent(const RequestFermentationModeEvent& event){
    sendEventToHost(event.header());
}

void SimpleScreenAbs::sendEvent(const RequestTargetTemperatureEvent& event){
    sendEventToHost(event.header());
}

SimpleScreenStates& SimpleScreenAbs::states(){
    return mStates.value();
}

void SimpleScreenAbs::init(ComponentHostIfc& host){

    // init gui
    mGui.init();

    mRenderDispatcher.registerHandler(renderEngineStatusTrampoline, this);
    mRenderDispatcher.registerHandler(renderTargetTemperatureTrampoline, this);
    mRenderDispatcher.registerHandler(renderMeasuredTemperatureTrampoline, this);
    mRenderDispatcher.registerHandler(renderMeasuredHumidityTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeaterPowerTrampoline, this);
    mRenderDispatcher.registerHandler(renderFanStatusTrampoline, this);

    // calls onInit()
    ScreenAbs::init(host);
}

void SimpleScreenAbs::render(){
    ScreenAbs::render();
    mDispatchedCount += mRenderDispatcher.getDirtyCount();
    mRenderDispatcher.dispatch();
}

void SimpleScreenAbs::becomeEnabled(){
    mGui.show();

    // mark all render functions dirty for complete redraw 
    markDirty(RenderFn::EngineStatus);
    markDirty(RenderFn::TargetTemperature);
    markDirty(RenderFn::MeasuredTemperature);
    markDirty(RenderFn::MeasuredHumidity);
    markDirty(RenderFn::HeaterPower);
    markDirty(RenderFn::FanStatus);

    ScreenAbs::becomeEnabled();
}

void SimpleScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

SimpleScreenGui::Objects& SimpleScreenAbs::gui(){
    return mGui.objects();
}

void SimpleScreenAbs::setBackgroundColor(uint32_t color){
    mGui.objects().root.setBgColor(lv_color_hex(color));
}

void SimpleScreenAbs::markDirty(RenderFn fn){
    mRenderDispatcher.markDirty(static_cast<size_t>(fn));
}

bool SimpleScreenAbs::isMarkedDirty(RenderFn fn) const {
    return mRenderDispatcher.isMarkedDirty(static_cast<size_t>(fn));
}

void SimpleScreenAbs::renderEngineStatusTrampoline(void* context){
    static_cast<SimpleScreenAbs*>(context)->onRenderEngineStatus();
}

void SimpleScreenAbs::renderTargetTemperatureTrampoline(void* context){
    static_cast<SimpleScreenAbs*>(context)->onRenderTargetTemperature();
}

void SimpleScreenAbs::renderMeasuredTemperatureTrampoline(void* context){
    static_cast<SimpleScreenAbs*>(context)->onRenderMeasuredTemperature();
}

void SimpleScreenAbs::renderMeasuredHumidityTrampoline(void* context){
    static_cast<SimpleScreenAbs*>(context)->onRenderMeasuredHumidity();
}

void SimpleScreenAbs::renderHeaterPowerTrampoline(void* context){
    static_cast<SimpleScreenAbs*>(context)->onRenderHeaterPower();
}

void SimpleScreenAbs::renderFanStatusTrampoline(void* context){
    static_cast<SimpleScreenAbs*>(context)->onRenderFanStatus();
}

} // namespace Garbox