// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"

LV_IMAGE_DECLARE(MainScreen_image_power_16px);
LV_IMAGE_DECLARE(MainScreen_image_temperature_16px);
LV_IMAGE_DECLARE(MainScreen_image_heat_16px);
LV_IMAGE_DECLARE(MainScreen_image_fan_16px);
LV_IMAGE_DECLARE(MainScreen_image_humidity_16px);
LV_IMAGE_DECLARE(MainScreen_image_rpm_16px);

namespace Garbox {

class MainScreenGui {
public:

    // ============================================================
    // === Component Structs ======================================
    // ============================================================

    struct SettingRow {
        SettingRow(LvObject& parent);
        LvObject body;
        LvLabel label;
        LvLabel value;

        SettingRow(const SettingRow&) = delete;
        SettingRow& operator=(const SettingRow&) = delete;
        SettingRow(SettingRow&&) = delete;
        SettingRow& operator=(SettingRow&&) = delete;
        ~SettingRow() = default;
    };

    struct InfoRowUnitText {
        InfoRowUnitText(LvObject& parent);
        LvObject body;
        LvImage icon;
        LvLabel value;
        LvLabel unit;

        InfoRowUnitText(const InfoRowUnitText&) = delete;
        InfoRowUnitText& operator=(const InfoRowUnitText&) = delete;
        InfoRowUnitText(InfoRowUnitText&&) = delete;
        InfoRowUnitText& operator=(InfoRowUnitText&&) = delete;
        ~InfoRowUnitText() = default;
    };

    struct InfoRowUnitIcon {
        InfoRowUnitIcon(LvObject& parent);
        LvObject body;
        LvImage icon;
        LvLabel value;
        LvImage unit;

        InfoRowUnitIcon(const InfoRowUnitIcon&) = delete;
        InfoRowUnitIcon& operator=(const InfoRowUnitIcon&) = delete;
        InfoRowUnitIcon(InfoRowUnitIcon&&) = delete;
        InfoRowUnitIcon& operator=(InfoRowUnitIcon&&) = delete;
        ~InfoRowUnitIcon() = default;
    };

    // ============================================================
    // === Objects Struct =========================================
    // ============================================================

    struct Objects {
        Objects();
        LvObject root;
        LvObject _namelessContainer_0;
        LvObject _namelessContainer_1;
        SettingRow settingPower;
        SettingRow settingTarget;
        SettingRow settingFan;
        SettingRow settingHeater;
        SettingRow settingBrightness;
        SettingRow settingCalibrate;
        SettingRow settingAdvanced;
        LvObject _namelessContainer_2;
        LvLabel _namelessLabel_0;
        InfoRowUnitText statusEngine;
        InfoRowUnitText statusTemperature;
        InfoRowUnitText statusHeat;
        InfoRowUnitText statusFan;
        LvObject _namelessContainer_3;
        LvLabel _namelessLabel_1;
        InfoRowUnitText sensorPower;
        InfoRowUnitText sensorTemperature;
        InfoRowUnitText sensorHumidity;
        InfoRowUnitIcon sensorFan;

        Objects(const Objects&) = delete;
        Objects& operator=(const Objects&) = delete;
        Objects(Objects&&) = delete;
        Objects& operator=(Objects&&) = delete;
        ~Objects() = default;
    };

    // ============================================================
    // === Gui Class ==============================================
    // ============================================================

    MainScreenGui();

    void init();
    void show();
    Objects& objects();

    MainScreenGui(const MainScreenGui&) = delete;
    MainScreenGui& operator=(const MainScreenGui&) = delete;
    MainScreenGui(MainScreenGui&&) = delete;
    MainScreenGui& operator=(MainScreenGui&&) = delete;
    ~MainScreenGui() = default;

private:
    Objects mObjects;
};

} // namespace