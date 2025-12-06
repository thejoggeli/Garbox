// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvLabel.h"

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
        LvLabel left;
        LvLabel value;
        LvLabel right;

        SettingRow(const SettingRow&) = delete;
        SettingRow& operator=(const SettingRow&) = delete;
        SettingRow(SettingRow&&) = delete;
        SettingRow& operator=(SettingRow&&) = delete;
        ~SettingRow() = default;
    };

    // ============================================================
    // === Objects Struct =========================================
    // ============================================================

    struct Objects {
        Objects();
        LvObject root;
        SettingRow settingPower;
        SettingRow settingTarget;

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
    void hide();
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