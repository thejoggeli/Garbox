// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"

LV_IMAGE_DECLARE(MainScreen_image_power_16px);
LV_IMAGE_DECLARE(MainScreen_image_temperature_16px);
LV_IMAGE_DECLARE(MainScreen_image_fan_16px);
LV_IMAGE_DECLARE(MainScreen_image_heat_16px);
LV_IMAGE_DECLARE(MainScreen_image_humidity_16px);

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

    struct InfoRow {
        InfoRow(LvObject& parent);
        LvObject body;
        LvImage icon;
        LvLabel value;
        LvLabel unit;

        InfoRow(const InfoRow&) = delete;
        InfoRow& operator=(const InfoRow&) = delete;
        InfoRow(InfoRow&&) = delete;
        InfoRow& operator=(InfoRow&&) = delete;
        ~InfoRow() = default;
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
        InfoRow statusEngine;
        InfoRow statusTemperature;
        InfoRow statusFan;
        InfoRow statusHeat;
        LvObject _namelessContainer_3;
        InfoRow sensorPower;
        InfoRow sensorTemperature;
        InfoRow sensorFan;
        InfoRow sensorHumidity;

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