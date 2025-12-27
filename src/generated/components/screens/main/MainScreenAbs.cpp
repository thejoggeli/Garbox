// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include "core/lvgl/LvglProvider.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs(): 
    ScreenAbs(ComponentId::MainScreen, ScreenId::Main),
    mGui(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()),
    mRenderDispatcher(static_cast<uint32_t>(RenderFn::Count)){}

MainScreenStates& MainScreenAbs::states(){
    return mStates.value();
}

void MainScreenAbs::init(ComponentHostIfc& host){

    // init gui
    mGui.init();

    mRenderDispatcher.registerHandler(renderFanInfoTrampoline, this);
    mRenderDispatcher.registerHandler(renderPowerInfoTrampoline, this);
    mRenderDispatcher.registerHandler(renderHumidityInfoTrampoline, this);
    mRenderDispatcher.registerHandler(renderStatusInfoTrampoline, this);
    mRenderDispatcher.registerHandler(renderMeasuredTemperatureLabelTrampoline, this);
    mRenderDispatcher.registerHandler(renderHeatpadPowerLabelTrampoline, this);
    mRenderDispatcher.registerHandler(renderTimeAxisTrampoline, this);
    mRenderDispatcher.registerHandler(renderTimeSeriesTrampoline, this);

    // calls onInit()
    ScreenAbs::init(host);
}

void MainScreenAbs::render(){
    ScreenAbs::render();
    mDispatchedCount += mRenderDispatcher.getDirtyCount();
    mRenderDispatcher.dispatch();
}

void MainScreenAbs::becomeEnabled(){
    mGui.show();

    // mark all render functions dirty for complete redraw 
    markDirty(RenderFn::FanInfo);
    markDirty(RenderFn::PowerInfo);
    markDirty(RenderFn::HumidityInfo);
    markDirty(RenderFn::StatusInfo);
    markDirty(RenderFn::MeasuredTemperatureLabel);
    markDirty(RenderFn::HeatpadPowerLabel);
    markDirty(RenderFn::TimeAxis);
    markDirty(RenderFn::TimeSeries);

    ScreenAbs::becomeEnabled();
}

void MainScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

MainScreenGui::Objects& MainScreenAbs::gui(){
    return mGui.objects();
}

void MainScreenAbs::setBackgroundColor(uint32_t color){
    mGui.objects().root.setBgColor(lv_color_hex(color));
}

void MainScreenAbs::markDirty(RenderFn fn){
    mRenderDispatcher.markDirty(static_cast<size_t>(fn));
}

bool MainScreenAbs::isMarkedDirty(RenderFn fn) const {
    return mRenderDispatcher.isMarkedDirty(static_cast<size_t>(fn));
}

void MainScreenAbs::renderFanInfoTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderFanInfo();
}

void MainScreenAbs::renderPowerInfoTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderPowerInfo();
}

void MainScreenAbs::renderHumidityInfoTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderHumidityInfo();
}

void MainScreenAbs::renderStatusInfoTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderStatusInfo();
}

void MainScreenAbs::renderMeasuredTemperatureLabelTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderMeasuredTemperatureLabel();
}

void MainScreenAbs::renderHeatpadPowerLabelTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderHeatpadPowerLabel();
}

void MainScreenAbs::renderTimeAxisTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderTimeAxis();
}

void MainScreenAbs::renderTimeSeriesTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onRenderTimeSeries();
}

} // namespace Garbox