// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/lvgl/objects/LvArc.h"
#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"

LV_IMAGE_DECLARE(SimpleScreen_image_power16);
LV_IMAGE_DECLARE(SimpleScreen_image_humidity16);
LV_IMAGE_DECLARE(SimpleScreen_image_heat16);
LV_IMAGE_DECLARE(SimpleScreen_image_fan16);

namespace Garbox {

class SimpleScreenGui {
public:

    // ============================================================
    // === Component Structs ======================================
    // ============================================================

    struct MetricCard {
        MetricCard(LvObject& parent);
        LvObject body;
        LvImage icon;
        LvObject _namelessContainer_0;
        LvLabel label;
        LvLabel value;

        MetricCard(const MetricCard&) = delete;
        MetricCard& operator=(const MetricCard&) = delete;
        MetricCard(MetricCard&&) = delete;
        MetricCard& operator=(MetricCard&&) = delete;
        ~MetricCard() = default;
    };

    // ============================================================
    // === Objects Struct =========================================
    // ============================================================

    struct Objects {
        Objects();
        LvObject root;
        LvObject wrapper;
        LvObject header;
        LvObject statusPill;
        LvImage statusIcon;
        LvLabel statusText;
        LvLabel screenTitle;
        LvLabel menuHint;
        LvObject content;
        LvObject gaugePanel;
        LvObject arcContainer;
        LvArc arcBg;
        LvArc arcMeasured;
        LvArc arcTarget;
        LvObject _namelessContainer_0;
        LvLabel measuredLabel;
        LvObject _namelessContainer_1;
        LvLabel measuredValue;
        LvLabel measuredUnit;
        LvLabel temperatureDelta;
        LvObject sidePanel;
        LvObject targetCard;
        LvLabel targetLabel;
        LvObject _namelessContainer_2;
        LvLabel targetValue;
        LvLabel targetUnit;
        LvLabel targetHint;
        MetricCard humidity;
        MetricCard heater;
        MetricCard fan;
        LvObject footer;
        LvLabel powerHint;
        LvLabel rangeHint;

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