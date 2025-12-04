#include "LvObject.h"
#include "core/assert/Assert.h"

namespace Garbox {

char LvObject::sBuffer[256];

LvObject::LvObject(lv_obj_t* raw):
    mRaw(raw){
    AssertExit(raw != nullptr, "LvObject", "got nullptr");
}

lv_obj_t* LvObject::raw() const {
    return mRaw;
}

void LvObject::setSize(int32_t widthPixels, int32_t heightPixels){
    lv_obj_set_size(mRaw, widthPixels, heightPixels);
}

void LvObject::setPosition(int32_t xPixels, int32_t yPixels){
    lv_obj_set_pos(mRaw, xPixels, yPixels);
}

void LvObject::getSize(int32_t& widthPixels, int32_t& heightPixels) const {
    widthPixels = lv_obj_get_width(mRaw);
    heightPixels = lv_obj_get_height(mRaw);
}

void LvObject::getPosition(int32_t& xPixels, int32_t& yPixels) const {
    xPixels = lv_obj_get_x(mRaw);
    yPixels = lv_obj_get_y(mRaw);
}

void LvObject::getArea(lv_area_t area) const {
    lv_obj_get_coords(mRaw, &area);
}

int32_t LvObject::getPositionX() const {
    return lv_obj_get_x(mRaw);
}

int32_t LvObject::getPositionY() const {
    return lv_obj_get_y(mRaw);
}

int32_t LvObject::getWidth() const {
    return lv_obj_get_width(mRaw);
}

int32_t LvObject::getHeight() const {
    return lv_obj_get_height(mRaw);
}

void LvObject::center() {
    lv_obj_center(mRaw);
}

void LvObject::setWidthContent(){
    lv_obj_set_width(mRaw, LV_SIZE_CONTENT);
}

void LvObject::setHeightContent(){
    lv_obj_set_height(mRaw, LV_SIZE_CONTENT);
}

void LvObject::setSizeContent(){
    lv_obj_set_size(mRaw, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
}

void LvObject::setFlexGrow(uint8_t grow){
    lv_obj_set_flex_grow(mRaw, grow);
}

void LvObject::addFlag(lv_obj_flag_t flag){
    lv_obj_add_flag(mRaw, flag);
}

void LvObject::clearFlag(lv_obj_flag_t flag){
    lv_obj_clear_flag(mRaw, flag);
}

void LvObject::setHidden(bool hidden){
    if(hidden){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_HIDDEN);
    }
}

void LvObject::setScrollable(bool scrollable){
    if(scrollable){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_SCROLLABLE);
    }
    else {
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_SCROLLABLE);
    }
}

void LvObject::setTextColor(lv_color_t color){
    lv_obj_set_style_text_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setFont(const lv_font_t* font){
    lv_obj_set_style_text_font(mRaw, font, LV_PART_MAIN);
}

void LvObject::setOpacity(uint8_t opacity){
    lv_obj_set_style_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setBgColor(lv_color_t color){
    lv_obj_set_style_bg_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setBgOpacity(uint8_t opacity) {
    lv_obj_set_style_bg_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setPad(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels){
    lv_obj_set_style_pad_left(mRaw, leftPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_right(mRaw, rightPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_top(mRaw, topPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(mRaw, bottomPixels, LV_PART_MAIN);
}

void LvObject::setBorder(int32_t widthPixels, lv_color_t color){
    lv_obj_set_style_border_width(mRaw, widthPixels, LV_PART_MAIN);
    lv_obj_set_style_border_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setRadius(int32_t radiusPixels){
    lv_obj_set_style_radius(mRaw, radiusPixels, LV_PART_MAIN);
}

void LvObject::setTextLetterSpace(int32_t spacePixels){
    lv_obj_set_style_text_letter_space(mRaw, spacePixels, LV_PART_MAIN);
}

void LvObject::setTextLineSpace(int32_t spacePixels){
    lv_obj_set_style_text_line_space(mRaw, spacePixels, LV_PART_MAIN);
}

void LvObject::setStyleProp(uint32_t prop, lv_style_value_t value){
    lv_obj_set_local_style_prop(mRaw, prop, value, LV_PART_MAIN);
}

} // namespace Garbox
