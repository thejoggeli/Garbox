#include "LvArc.h"

namespace Garbox {

LvArc::LvArc(LvObject& parent)
    : LvObject(lv_arc_create(parent.raw())) {
    // constructor body
}

void LvArc::setArcValue(int32_t value) {
    lv_arc_set_value(raw(), value);
}

void LvArc::setArcRange(int32_t min, int32_t max) {
    lv_arc_set_range(raw(), min, max);
}

void LvArc::setArcRotation(uint16_t rotation) {
    lv_arc_set_rotation(raw(), rotation);
}

void LvArc::setArcBgAngles(uint16_t start, uint16_t end) {
    lv_arc_set_bg_angles(raw(), start, end);
}

void LvArc::setArcAngles(uint16_t start, uint16_t end) {
    lv_arc_set_angles(raw(), start, end);
}

void LvArc::setArcMode(lv_arc_mode_t mode) {
    lv_arc_set_mode(raw(), mode);
}

void LvArc::setArcColor(lv_color_t color) {
    lv_obj_set_style_arc_color(raw(), color, LV_PART_INDICATOR);
}

void LvArc::setArcWidth(int32_t width) {
    lv_obj_set_style_arc_width(raw(), width, LV_PART_INDICATOR);
}

void LvArc::setArcRounded(bool enabled) {
    lv_obj_set_style_arc_rounded(raw(), enabled, LV_PART_INDICATOR);
}

void LvArc::setArcOpa(lv_opa_t opa) {
    lv_obj_set_style_arc_opa(raw(), opa, LV_PART_INDICATOR);
}

void LvArc::setArcImageSrc(const void* src) {
    lv_obj_set_style_arc_image_src(raw(), src, LV_PART_INDICATOR);
}

void LvArc::setArcBgColor(lv_color_t color) {
    lv_obj_set_style_arc_color(raw(), color, LV_PART_MAIN);
}

void LvArc::setArcBgWidth(int32_t width) {
    lv_obj_set_style_arc_width(raw(), width, LV_PART_MAIN);
}

void LvArc::setArcBgRounded(bool enabled) {
    lv_obj_set_style_arc_rounded(raw(), enabled, LV_PART_MAIN);
}

void LvArc::setArcBgOpa(lv_opa_t opa) {
    lv_obj_set_style_arc_opa(raw(), opa, LV_PART_MAIN);
}

void LvArc::setArcBgImageSrc(const void* src) {
    lv_obj_set_style_arc_image_src(raw(), src, LV_PART_MAIN);
}

void LvArc::removeKnob(){
    lv_obj_remove_style(raw(), NULL, LV_PART_KNOB);
}

} // namespace Garbox
