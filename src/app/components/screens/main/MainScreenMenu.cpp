#include "MainScreenMenu.h"

#include <algorithm>
#include <cstdio>
#include "core/util/math/MathUtils.h"

namespace Garbox {

static const char* HistoryLabels[] = {
    "1 min", "4 min", "16 min", "1 hr", "4 hrs", "12 hrs", "1 day", "4 days"
};

static constexpr uint32_t HistoryCount = sizeof(HistoryLabels) / sizeof(HistoryLabels[0]);

static constexpr uint32_t VisibleRows = 6;
static constexpr uint32_t ColorTextDefault = 0xD9E2E8;
static constexpr uint32_t ColorTextActive = 0x081018;
static constexpr uint32_t ColorBgDefault = 0x0D141A;
static constexpr uint32_t ColorBgActive = 0x8AB4F4;
static constexpr uint32_t ColorTextEdit = 0x081018;
static constexpr uint32_t ColorBgEdit = 0xD1B254;

MainScreenMenu::MainScreenMenu(LvObject& container, void* callbackCtx, ValueChangedCallback valueChangedCallback):
    mMenu(
        container, 
        ColorTextDefault, 
        ColorTextActive, 
        ColorBgDefault, 
        ColorBgActive, 
        ColorTextEdit,
        ColorBgEdit,
        RowIndex::Count,
        VisibleRows,
        this, 
        valueToText, 
        changeValue
    ),
    mCallbackCtx(callbackCtx),
    mValueChangedCallback(valueChangedCallback){

    mMenu.addRow("Power", 0);
    mMenu.addRow("Target", 0);
    mMenu.addRow("Fan mode", 0);
    mMenu.addRow("Fan speed", 0);
    mMenu.addRow("Brightness", 50);
    mMenu.addRow("History", 0);
    mMenu.addRow("Plot min", 20);
    mMenu.addRow("Plot max", 40);
    mMenu.addRow("Diagnostics", 0);
    mMenu.addRow("Event log", 0);
    mMenu.addRow("State log", 0);
    mMenu.addRow("Home", 0);
    mMenu.setSelectedRow(0);
    mMenu.setPad(1, 1, 2, 2);
}

void MainScreenMenu::render(){
    mMenu.render();
}

MainScreenMenu::RowIndex MainScreenMenu::onButtonInput(){
    if(!mMenu.hasSelection()){
        return RowIndex::Count;
    }
    const RowIndex selected = static_cast<RowIndex>(mMenu.getSelectedRow());
    if(mMenu.isLocked()){
        mMenu.toggleLock();
        return RowIndex::Count;
    }
    if(selectedRowIsToggle()){
        mMenu.changeSelectedValue(+1);
        return RowIndex::Count;
    }
    if(selectedRowIsAction()){
        return selected;
    }
    mMenu.toggleLock();
    return RowIndex::Count;
}

bool MainScreenMenu::isEditing() const{
    return mMenu.isLocked();
}

MainScreenMenu::RowIndex MainScreenMenu::getSelectedRow() const{
    if(!mMenu.hasSelection()){
        return RowIndex::Count;
    }
    return static_cast<RowIndex>(mMenu.getSelectedRow());
}

bool MainScreenMenu::selectedRowIsToggle() const{
    const RowIndex selected = getSelectedRow();
    return selected == RowIndex::Power || selected == RowIndex::FanMode;
}

bool MainScreenMenu::selectedRowIsAction() const{
    const RowIndex selected = getSelectedRow();
    return selected >= RowIndex::Diagnostics && selected < RowIndex::Count;
}

void MainScreenMenu::onEncoderInput(int32_t steps){
    if(steps > 0){
        for(int32_t i = 0; i < steps; i++){
            mMenu.nextValue();
            mMenu.selectNextRow();
        }
    }
    else if(steps < 0){
        for(int32_t i = 0; i < -steps; i++){
            mMenu.previousValue();
            mMenu.selectPreviousRow();
        }
    }
}

int32_t MainScreenMenu::changeValue(void* callbackCtx, uint8_t rowIndex, int32_t currentValue, int32_t delta){
    int32_t newValue;
    switch(rowIndex){
        case Power:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta, 0, 2);
            break;
        case Target:
            newValue = MathUtils::Clamp<int32_t>(currentValue + delta, 150, 500);
            break;
        case FanMode:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta, 0, 2);
            break;
        case FanSpeed:
        case Brightness:
            newValue = MathUtils::Clamp<int32_t>(currentValue + delta * 5, 0, 100);
            break;
        case History:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta, 0, static_cast<int32_t>(HistoryCount));
            break;
        case PlotMinimum:
            newValue = currentValue == 10 ? 20 : 10;
            break;
        case PlotMaximum:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta * 10, 40, 70);
            break;
        default:
            newValue = currentValue;
            break;
    }
    MainScreenMenu* self = static_cast<MainScreenMenu*>(callbackCtx);
    if(self->mValueChangedCallback && currentValue != newValue){
        self->mValueChangedCallback(self->mCallbackCtx, static_cast<RowIndex>(rowIndex), currentValue, newValue);
    }
    return newValue;
}

void MainScreenMenu::valueToText(void* callbackCtx, uint8_t rowIndex, int32_t value, char* outText, uint32_t maxLen){
    switch(rowIndex){
        case Power:
            std::snprintf(outText, maxLen, value ? "On" : "Off");
            break;

        case Target:
            std::snprintf(outText, maxLen, "%d.%d°C", value / 10, value % 10);
            break;

        case History:
            std::snprintf(outText, maxLen, "%s", HistoryLabels[value]);
            break;

        case PlotMinimum:
        case PlotMaximum:
            std::snprintf(outText, maxLen, "%d°C", value);
            break;

        case FanMode:
            std::snprintf(outText, maxLen, value ? "Auto" : "Manual");
            break;

        case FanSpeed:
        case Brightness:
            std::snprintf(outText, maxLen, "%d%%", value);
            break;

        case Diagnostics:
        case EventLog:
        case StateLog:
            std::snprintf(outText, maxLen, ">");
            break;

        case Home:
            std::snprintf(outText, maxLen, "BACK");
            break;

        default:
            outText[0] = '\0';
            break;
    }
}

void MainScreenMenu::setPowerEnabled(bool enabled){
    mMenu.setValue(RowIndex::Power, enabled ? 1 : 0);
}

void MainScreenMenu::setTargetTemperatureCelsius(float temperature){
    mMenu.setValue(RowIndex::Target, static_cast<int32_t>(temperature * 10.0f + 0.5f));
}

void MainScreenMenu::setHistoryIndex(uint8_t index){
    mMenu.setValue(RowIndex::History, index);
}

void MainScreenMenu::setPlotRange(int32_t minimumCelsius, int32_t maximumCelsius){
    mMenu.setValue(RowIndex::PlotMinimum, minimumCelsius);
    mMenu.setValue(RowIndex::PlotMaximum, maximumCelsius);
}

void MainScreenMenu::setFanAuto(bool isAuto){
    mMenu.setValue(RowIndex::FanMode, isAuto ? 1 : 0);
}

void MainScreenMenu::setFanManualSpeed(float speed){
    mMenu.setValue(RowIndex::FanSpeed, static_cast<int32_t>(speed * 100.0f + 0.5f));
}

void MainScreenMenu::setBrightness(float brightness){
    mMenu.setValue(RowIndex::Brightness, static_cast<int32_t>(brightness * 100.0f + 0.5f));
}

bool MainScreenMenu::isPowerEnabled() const{
    return mMenu.getValue(RowIndex::Power) != 0;
}

float MainScreenMenu::getTargetTemperatureCelsius() const{
    return static_cast<float>(mMenu.getValue(RowIndex::Target)) * 0.1f;
}

uint32_t MainScreenMenu::getHistoryIndex() const{
    return static_cast<uint32_t>(mMenu.getValue(RowIndex::History));
}

int32_t MainScreenMenu::getPlotMinimumCelsius() const{
    return mMenu.getValue(RowIndex::PlotMinimum);
}

int32_t MainScreenMenu::getPlotMaximumCelsius() const{
    return mMenu.getValue(RowIndex::PlotMaximum);
}

bool MainScreenMenu::isFanAuto() const{
    return mMenu.getValue(RowIndex::FanMode) != 0;
}

float MainScreenMenu::getFanManualSpeed() const{
    return static_cast<float>(mMenu.getValue(RowIndex::FanSpeed)) * 0.01f;
}

float MainScreenMenu::getBrightness() const{
    return static_cast<float>(mMenu.getValue(RowIndex::Brightness)) * 0.01f;
}

} // namespace Garbox
