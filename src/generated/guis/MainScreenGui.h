// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvChart.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"

LV_IMAGE_DECLARE(MainScreen_image_arrow_up);
LV_IMAGE_DECLARE(MainScreen_image_arrow_down);
LV_IMAGE_DECLARE(MainScreen_image_fan16);
LV_IMAGE_DECLARE(MainScreen_image_humidity16);
LV_IMAGE_DECLARE(MainScreen_image_power16);
LV_IMAGE_DECLARE(MainScreen_image_heat16);

namespace Garbox {

class MainScreenGui {
public:

    // ============================================================
    // === Component Structs ======================================
    // ============================================================

    struct GraphComponent {
        GraphComponent(LvObject& parent);
        LvObject body;
        LvObject labelContainer;
        LvObject _namelessContainer_0;
        LvChart chart;
        LvLabel value;

        GraphComponent(const GraphComponent&) = delete;
        GraphComponent& operator=(const GraphComponent&) = delete;
        GraphComponent(GraphComponent&&) = delete;
        GraphComponent& operator=(GraphComponent&&) = delete;
        ~GraphComponent() = default;
    };

    struct MenuRow {
        MenuRow(LvObject& parent);
        LvObject body;
        LvLabel label;
        LvLabel value;

        MenuRow(const MenuRow&) = delete;
        MenuRow& operator=(const MenuRow&) = delete;
        MenuRow(MenuRow&&) = delete;
        MenuRow& operator=(MenuRow&&) = delete;
        ~MenuRow() = default;
    };

    struct InfoTile {
        InfoTile(LvObject& parent);
        LvObject body;
        LvImage icon;
        LvLabel value;

        InfoTile(const InfoTile&) = delete;
        InfoTile& operator=(const InfoTile&) = delete;
        InfoTile(InfoTile&&) = delete;
        InfoTile& operator=(InfoTile&&) = delete;
        ~InfoTile() = default;
    };

    // ============================================================
    // === Objects Struct =========================================
    // ============================================================

    struct Objects {
        Objects();
        LvObject root;
        LvObject wrapper;
        LvObject _namelessContainer_0;
        GraphComponent tempGraph;
        GraphComponent powerGraph;
        LvObject timeAxis;
        LvLabel t4;
        LvLabel t3;
        LvLabel t2;
        LvLabel t1;
        LvLabel t0;
        LvObject _namelessContainer_1;
        LvObject systemStateBg;
        LvLabel systemState;
        LvObject menuContainer;
        LvImage menuArrUp;
        LvObject _namelessContainer_2;
        LvObject _namelessContainer_3;
        MenuRow _namelessMenuRow_0;
        MenuRow _namelessMenuRow_1;
        MenuRow _namelessMenuRow_2;
        MenuRow _namelessMenuRow_3;
        MenuRow _namelessMenuRow_4;
        LvObject _namelessContainer_4;
        LvImage menuArrDown;
        LvObject infoTiles;
        InfoTile fanInfo;
        InfoTile humidInfo;
        InfoTile powerInfo;
        InfoTile timeInfo;

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