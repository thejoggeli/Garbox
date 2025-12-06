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

// ==============================================================================
// raw positioning
// ==============================================================================

void LvObject::setPosition(int32_t xPixels, int32_t yPixels){
    lv_obj_set_pos(mRaw, xPixels, yPixels);
}

void LvObject::setPositionX(int32_t xPixels){
    lv_obj_set_pos(mRaw, xPixels, getPositionY());
}

void LvObject::setPositionY(int32_t yPixels){
    lv_obj_set_pos(mRaw, getPositionX(), yPixels);
}

void LvObject::getPosition(int32_t& xPixels, int32_t& yPixels) const {
    xPixels = lv_obj_get_x(mRaw);
    yPixels = lv_obj_get_y(mRaw);
}

int32_t LvObject::getPositionX() const {
    return lv_obj_get_x(mRaw);
}

int32_t LvObject::getPositionY() const {
    return lv_obj_get_y(mRaw);
}

// ==============================================================================
// area
// ==============================================================================

void LvObject::getArea(lv_area_t& area) const {
    lv_obj_get_coords(mRaw, &area);
}

// ==============================================================================
// raw sizing
// ==============================================================================

void LvObject::setRawSize(int32_t widthPixels, int32_t heightPixels){
    lv_obj_set_size(mRaw, widthPixels, heightPixels);
}

void LvObject::setRawWidth(int32_t widthPixels){
    lv_obj_set_width(mRaw, widthPixels);
}

void LvObject::setRawHeight(int32_t heightPixels){
    lv_obj_set_height(mRaw, heightPixels);
}

void LvObject::getRawSize(int32_t& widthPixels, int32_t& heightPixels) const {
    widthPixels = lv_obj_get_width(mRaw);
    heightPixels = lv_obj_get_height(mRaw);
}

int32_t LvObject::getRawWidth() const {
    return lv_obj_get_width(mRaw);
}

int32_t LvObject::getRawHeight() const {
    return lv_obj_get_height(mRaw);
}

// ==============================================================================
// style based sizing
// ==============================================================================

void LvObject::setSize(lv_coord_t width, lv_coord_t height){
    lv_obj_set_style_size(mRaw, width, height, LV_PART_MAIN);
}

void LvObject::setWidth(lv_coord_t value){
    lv_obj_set_style_width(mRaw, value, LV_PART_MAIN);
}

void LvObject::setHeight(lv_coord_t value){
    lv_obj_set_style_height(mRaw, value, LV_PART_MAIN);
}

void LvObject::getSize(int32_t& widthPixels, int32_t& heightPixels) const {
    widthPixels = lv_obj_get_style_width(mRaw, LV_PART_MAIN);
    heightPixels = lv_obj_get_style_height(mRaw, LV_PART_MAIN);
}

int32_t LvObject::getWidth() const {
    return lv_obj_get_style_width(mRaw, LV_PART_MAIN);
}

int32_t LvObject::getHeight() const {
    return lv_obj_get_style_height(mRaw, LV_PART_MAIN);
}

// ==============================================================================
// alignment helpers
// ==============================================================================

void LvObject::center() {
    lv_obj_center(mRaw);
}

// ==============================================================================
// content sizing helpers
// ==============================================================================

void LvObject::setWidthContent(){
    lv_obj_set_width(mRaw, LV_SIZE_CONTENT);
}

void LvObject::setHeightContent(){
    lv_obj_set_height(mRaw, LV_SIZE_CONTENT);
}

void LvObject::setSizeContent(){
    lv_obj_set_size(mRaw, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
}

// ==============================================================================
// layout
// ==============================================================================

void LvObject::setLayout(lv_layout_t layout){
    lv_obj_set_layout(mRaw, layout);
}

// ==============================================================================
// flex
// ==============================================================================

void LvObject::setFlexFlow(lv_flex_flow_t flow){
    lv_obj_set_flex_flow(mRaw, flow);
}

void LvObject::setFlexGrow(uint8_t grow){
    lv_obj_set_flex_grow(mRaw, grow);
}

// ==============================================================================
// grid
// ==============================================================================

void LvObject::setGridColumns(const lv_coord_t* columns){
    lv_obj_set_style_grid_column_dsc_array(mRaw, columns, LV_PART_MAIN);
}

void LvObject::setGridRows(const lv_coord_t* rows){
    lv_obj_set_style_grid_row_dsc_array(mRaw, rows, LV_PART_MAIN);
}

void LvObject::setGridCell(lv_coord_t col, lv_coord_t row, lv_coord_t col_span, lv_coord_t row_span){
    lv_obj_set_grid_cell(mRaw, LV_GRID_ALIGN_STRETCH, col, col_span, LV_GRID_ALIGN_STRETCH, row, row_span);
}

// ==============================================================================
// flags
// ==============================================================================

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

// ==============================================================================
// scrolling
// ==============================================================================

void LvObject::setScrollable(bool scrollable){
    if(scrollable){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_SCROLLABLE);
    }
    else {
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_SCROLLABLE);
    }
}

void LvObject::setScrollOne(bool enabled){
    if(enabled){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_SCROLL_ONE);
    }
    else{
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_SCROLL_ONE);
    }
}

void LvObject::setScrollChain(bool enabled){
    if(enabled){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_SCROLL_CHAIN);
    }
    else{
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_SCROLL_CHAIN);
    }
}

void LvObject::setScrollMomentum(bool enabled){
    if(enabled){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    }
    else{
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    }
}

void LvObject::setScrollElastic(bool enabled){
    if(enabled){
        lv_obj_add_flag(mRaw, LV_OBJ_FLAG_SCROLL_ELASTIC);
    }
    else{
        lv_obj_clear_flag(mRaw, LV_OBJ_FLAG_SCROLL_ELASTIC);
    }
}

void LvObject::setScrollDirection(lv_dir_t dir){
    lv_obj_set_scroll_dir(mRaw, dir);
}

// ==============================================================================
// style helpers
// ==============================================================================

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

void LvObject::setBorder(int32_t widthPixels, lv_color_t color){
    lv_obj_set_style_border_width(mRaw, widthPixels, LV_PART_MAIN);
    lv_obj_set_style_border_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setBorderWidth(int32_t widthPixels){
    lv_obj_set_style_border_width(mRaw, widthPixels, LV_PART_MAIN);
}

void LvObject::setBorderColor(lv_color_t color){
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

// ==============================================================================
// padding
// ==============================================================================

void LvObject::setPad(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels){
    lv_obj_set_style_pad_left(mRaw, leftPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_right(mRaw, rightPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_top(mRaw, topPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(mRaw, bottomPixels, LV_PART_MAIN);
}

void LvObject::setPadLeft(int32_t leftPixels){
    lv_obj_set_style_pad_left(mRaw, leftPixels, LV_PART_MAIN);
}

void LvObject::setPadRight(int32_t rightPixels){
    lv_obj_set_style_pad_right(mRaw, rightPixels, LV_PART_MAIN);
}

void LvObject::setPadTop(int32_t topPixels){
    lv_obj_set_style_pad_top(mRaw, topPixels, LV_PART_MAIN);
}

void LvObject::setPadBottom(int32_t bottomPixels){
    lv_obj_set_style_pad_bottom(mRaw, bottomPixels, LV_PART_MAIN);
}

void LvObject::setPadX(int32_t xPixels){
    lv_obj_set_style_pad_left(mRaw, xPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_right(mRaw, xPixels, LV_PART_MAIN);
}

void LvObject::setPadY(int32_t yPixels){
    lv_obj_set_style_pad_top(mRaw, yPixels, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(mRaw, yPixels, LV_PART_MAIN);
}

// ==============================================================================
// style helper
// ==============================================================================

void LvObject::setStyleProp(uint32_t prop, lv_style_value_t value){
    lv_obj_set_local_style_prop(mRaw, prop, value, LV_PART_MAIN);
}

} // namespace Garbox
