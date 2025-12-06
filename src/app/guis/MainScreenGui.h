// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/lvgl/objects/LvImage.h"

namespace Garbox {

LV_IMAGE_DECLARE(MainScreen_image_power_16px);
LV_IMAGE_DECLARE(MainScreen_image_fan_16px);
LV_IMAGE_DECLARE(MainScreen_image_heat_16px);
LV_IMAGE_DECLARE(MainScreen_image_temperature_16px);
LV_IMAGE_DECLARE(MainScreen_image_humidity_16px);

class MainScreenGui {
public:

    // ============================================================
    // === Component Structs ======================================
    // ============================================================

    struct SettingRow {
        SettingRow(LvObject& parent);
        LvObject body;
        LvLabel label;
        LvLabel left;
        LvLabel value;
        LvLabel right;

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
        LvObject wrapper;
        LvObject leftPane;
        SettingRow settingPower;
        SettingRow settingTarget;
        SettingRow settingFan;
        SettingRow settingHeater;
        SettingRow settingCalibrate;
        SettingRow settingAdvanced;
        LvObject middlePane;
        LvLabel _namelessLabel_0;
        LvObject _namelessContainer_0;
        InfoRow _namelessInfoRow_0;
        InfoRow _namelessInfoRow_1;
        InfoRow _namelessInfoRow_2;
        LvObject rightPane;
        LvLabel _namelessLabel_1;
        LvObject _namelessContainer_1;
        InfoRow fanRpm;
        InfoRow temperature;
        InfoRow humidity;
        InfoRow power;

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