#include "MainScreenMenu.h"

#include <algorithm>
#include <cstdio>
#include "core/util/math/MathUtils.h"

namespace Garbox {

static const char* HistoryLabels[] = {
    "1 min", "4 min", "16 min", "1 hr", "4 hrs", "12 hrs", "1 day", "4 days"
};

static constexpr uint32_t HistoryCount = sizeof(HistoryLabels) / sizeof(HistoryLabels[0]);


static constexpr uint32_t VisibleRows = 5;
static constexpr uint32_t ColorTextDefault = 0xFFFFFF;
static constexpr uint32_t ColorTextActive = 0x0;
static constexpr uint32_t ColorBgDefault = 0x0;
static constexpr uint32_t ColorBgActive = 0xFFFFFF;

MainScreenMenu::MainScreenMenu(LvObject& container, void* callbackCtx, ValueChangedCallback valueChangedCallback):
    mMenu(
        container, 
        ColorTextDefault, 
        ColorTextActive, 
        ColorBgDefault, 
        ColorBgActive, 
        RowIndex::Count,
        VisibleRows,
        this, 
        valueToText, 
        changeValue
    ){

    mMenu.addRow("Power", 0);
    mMenu.addRow("Target", 0);
    mMenu.addRow("History", 0);
    mMenu.addRow("Fan", 0);
    mMenu.addRow("Calibrate", 0);
    mMenu.addRow("About", 0);
    mMenu.addRow("Debug", 0);
    mMenu.setSelectedRow(0);
    mMenu.setPad(1,1,1,1);
}

void MainScreenMenu::render(){
    mMenu.render();
}

bool MainScreenMenu::onEncoderInput(int32_t steps, bool buttonAction){
    bool changed = false;
    if(buttonAction){
        mMenu.toggleLock();
        changed = true;
    }
    else if(steps > 0){
        for(int32_t i = 0; i < steps; i++){
            mMenu.nextValue();
            mMenu.selectNextRow();
        }
        changed = true;
    }
    else if(steps < 0){
        for(int32_t i = 0; i < -steps; i++){
            mMenu.previousValue();
            mMenu.selectPreviousRow();
        }
        changed = true;
    }
    return changed;
}

int32_t MainScreenMenu::changeValue(void* callbackCtx, uint8_t rowIndex, int32_t currentValue, int32_t delta){
    int32_t newValue;
    switch(rowIndex){
        case Power:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta, 0, 2);
            break;
        case Target:
            newValue = MathUtils::Clamp<int32_t>(currentValue + delta, 200, 400);
            break;
        case History:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta, 0, static_cast<int32_t>(HistoryCount));
            break;
        case Fan:
            newValue = MathUtils::Wrap<int32_t>(currentValue + delta, 0, 2);
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

        case Fan:
            std::snprintf(outText, maxLen, value ? "Auto" : "Off");
            break;

        case Calibrate:
        case About:
        case Debug:
            std::snprintf(outText, maxLen, "Open");
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

void MainScreenMenu::setFanAuto(bool isAuto){
    mMenu.setValue(RowIndex::Fan, isAuto ? 1 : 0);
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

bool MainScreenMenu::isFanAuto() const{
    return mMenu.getValue(RowIndex::Fan) != 0;
}

} // namespace Garbox
