// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvArc.h"
#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"

LV_IMAGE_DECLARE(SimpleScreen_image_power16);
LV_IMAGE_DECLARE(SimpleScreen_image_temperature16);
LV_IMAGE_DECLARE(SimpleScreen_image_humidity16);
LV_IMAGE_DECLARE(SimpleScreen_image_heat16);

namespace Garbox {

class SimpleScreenGui {
public:

    // ============================================================
    // === Component Structs ======================================
    // ============================================================

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
        LvObject arcContainer;
        LvArc arcBg;
        LvArc arcMeasured;
        LvArc arcMeasuredEnd;
        LvArc arcMeasuredBorder;
        LvArc arcTarget;
        LvArc arcTargetEnd;
        LvArc arcTargetBorder;
        LvObject _namelessContainer_1;
        LvLabel tempLabel;
        LvLabel tempValue;
        LvLabel tempUnit;
        LvObject infoTiles;
        InfoTile onoffInfo;
        InfoTile tempInfo;
        InfoTile humidInfo;
        InfoTile heatInfo;

        Objects(const Objects&) = delete;
        Objects& operator=(const Objects&) = delete;
        Objects(Objects&&) = delete;
        Objects& operator=(Objects&&) = delete;
        ~Objects() = default;
    };

    // ============================================================
    // === Gui Class ==============================================
    // ============================================================

    SimpleScreenGui();

    void init();
    void show();
    Objects& objects();

    SimpleScreenGui(const SimpleScreenGui&) = delete;
    SimpleScreenGui& operator=(const SimpleScreenGui&) = delete;
    SimpleScreenGui(SimpleScreenGui&&) = delete;
    SimpleScreenGui& operator=(SimpleScreenGui&&) = delete;
    ~SimpleScreenGui() = default;

private:
    Objects mObjects;
};

} // namespace