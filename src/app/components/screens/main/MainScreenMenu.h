#pragma once

#include <cstdint>

#include "core/lvgl/helpers/menu/RowMenu.h"
#include "core/lvgl/objects/LvObject.h"

namespace Garbox {

class MainScreenMenu {
public:

    enum RowIndex : uint8_t {
        Power = 0,
        Target,
        History,
        Fan,
        Calibrate,
        About,
        Debug,
        Count
    };

    using ValueChangedCallback = void (*)(
        void* callbackCtx,
        RowIndex index,
        int32_t oldValue,
        int32_t newValue
    );

    MainScreenMenu(LvObject& container, void* callbackCtx, ValueChangedCallback valueChangedCallback);

    void render();
    bool onEncoderInput(int32_t steps, bool buttonAction);

    void setPowerEnabled(bool enabled);
    void setTargetTemperatureCelsius(float temperature);
    void setHistoryIndex(uint8_t index);
    void setFanAuto(bool isAuto);

    bool isPowerEnabled() const;
    float getTargetTemperatureCelsius() const;
    uint32_t getHistoryIndex() const;
    bool isFanAuto() const;

private:

    static int32_t changeValue(void* callbackCtx, uint8_t rowIndex, int32_t currentValue, int32_t delta);
    static void valueToText(void* callbackCtx, uint8_t rowIndex, int32_t value, char* outText, uint32_t maxLen);

    RowMenu mMenu;
    void* mCallbackCtx = nullptr;
    ValueChangedCallback mValueChangedCallback = nullptr;

};

} // namespace Garbox
