#include "LvObject.h"
#include "core/assert/Assert.h"

namespace Garbox {

char LvObject::sBuffer[256];

LvObject::LvObject(lv_obj_t* raw) : mRaw(raw){
    AssertExit(mRaw != nullptr, "LvObject", "got nullptr");
}

LvObject::LvObject() : LvObject(lv_obj_create(nullptr)){
    AssertExit(mRaw != nullptr, "LvObject", "got nullptr");
}

LvObject::LvObject(LvObject& parent) : LvObject(lv_obj_create(parent.raw())) {
    AssertExit(mRaw != nullptr, "LvObject", "got nullptr");
}

LvObject::~LvObject(){
    if(mRaw != nullptr){
        lv_obj_del(mRaw);
        mRaw = nullptr;
    }
}

LvObject::LvObject(LvObject&& other) noexcept : mRaw(other.mRaw){
    other.mRaw = nullptr;
}

LvObject& LvObject::operator=(LvObject&& other) noexcept {
    if(this != &other){
        if(mRaw != nullptr){
            lv_obj_del(mRaw);
        }

        mRaw = other.mRaw;
        other.mRaw = nullptr;
    }
    return *this;
}

lv_obj_t* LvObject::raw() const {
    return mRaw;
}

void LvObject::free(){
    AssertExit(mRaw != nullptr, "LvObject", "free nullptr");
    lv_obj_del(mRaw);
    mRaw = nullptr;
}

void LvObject::setScreen(){
    lv_disp_load_scr(mRaw);
}

// ==============================================================================
// ordering
// ==============================================================================

void LvObject::moveBackground(){
    lv_obj_move_background(mRaw);
}

void LvObject::moveForeground(){
    lv_obj_move_foreground(mRaw);
}

void LvObject::moveToIndex(int32_t index){
    lv_obj_move_to_index(mRaw, index);
}

int32_t LvObject::getIndex(){
    return lv_obj_get_index(mRaw);
}

// ==============================================================================
// position getters
// ==============================================================================

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
// width and height getters
// ==============================================================================

int32_t LvObject::getRawWidth() const {
    return lv_obj_get_width(mRaw);
}

int32_t LvObject::getRawHeight() const {
    return lv_obj_get_height(mRaw);
}

int32_t LvObject::getWidth() const {
    return lv_obj_get_style_width(mRaw, LV_PART_MAIN);
}

int32_t LvObject::getHeight() const {
    return lv_obj_get_style_height(mRaw, LV_PART_MAIN);
}

int32_t LvObject::getContentWidth() const {
    return lv_obj_get_content_width(mRaw);
}

int32_t LvObject::getContentHeight() const {
    return lv_obj_get_content_height(mRaw);
}

// ==============================================================================
// size getters
// ==============================================================================

void LvObject::getRawSize(int32_t& widthPixels, int32_t& heightPixels) const {
    widthPixels = lv_obj_get_width(mRaw);
    heightPixels = lv_obj_get_height(mRaw);
}

void LvObject::getSize(int32_t& widthPixels, int32_t& heightPixels) const {
    widthPixels = lv_obj_get_style_width(mRaw, LV_PART_MAIN);
    heightPixels = lv_obj_get_style_height(mRaw, LV_PART_MAIN);
}

// ==============================================================================
// coords getters
// ==============================================================================

void LvObject::getCoords(lv_area_t& area) const {
    lv_obj_get_coords(mRaw, &area);
}

void LvObject::getContentCoords(lv_area_t& area) const {
    lv_obj_get_content_coords(mRaw, &area);
}

// ==============================================================================
// raw positioning
// ==============================================================================

void LvObject::setPosition(int32_t xPixels, int32_t yPixels){
    lv_obj_set_pos(mRaw, xPixels, yPixels);
}

void LvObject::setPositionX(int32_t xPixels){
    lv_obj_set_x(mRaw, xPixels);
}

void LvObject::setPositionY(int32_t yPixels){
    lv_obj_set_y(mRaw, yPixels);
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

// ==============================================================================
// alignment helpers
// ==============================================================================

void LvObject::center() {
    lv_obj_center(mRaw);
}

void LvObject::setAlign(lv_align_t alignment, int32_t xOffset, int32_t yOffset){
    lv_obj_align(mRaw, alignment, xOffset, yOffset);
}

// ==============================================================================
// content sizing helpers
// ==============================================================================

void LvObject::setRawWidthContent(){
    lv_obj_set_width(mRaw, LV_SIZE_CONTENT);
}

void LvObject::setRawHeightContent(){
    lv_obj_set_height(mRaw, LV_SIZE_CONTENT);
}

void LvObject::setRawSizeContent(){
    lv_obj_set_size(mRaw, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
}

void LvObject::setWidthContent(){
    lv_obj_set_style_width(mRaw, LV_SIZE_CONTENT, LV_PART_MAIN);
}

void LvObject::setHeightContent(){
    lv_obj_set_style_height(mRaw, LV_SIZE_CONTENT, LV_PART_MAIN);
}

void LvObject::setSizeContent(){
    lv_obj_set_style_size(mRaw, LV_SIZE_CONTENT, LV_SIZE_CONTENT, LV_PART_MAIN);
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

void LvObject::setFlexAlign(lv_flex_align_t main_place, lv_flex_align_t cross_place, lv_flex_align_t track_place){
    lv_obj_set_flex_align(mRaw, main_place, cross_place, track_place);
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
// transform
// ==============================================================================

void LvObject::setTransformWidth(int32_t width) {
    lv_obj_set_style_transform_width(mRaw, width, LV_PART_MAIN);
}

void LvObject::setTransformHeight(int32_t height) {
    lv_obj_set_style_transform_height(mRaw, height, LV_PART_MAIN);
}

void LvObject::setTransformPivot(int32_t x, int32_t y) {
    lv_obj_set_style_transform_pivot_x(mRaw, x, LV_PART_MAIN);
    lv_obj_set_style_transform_pivot_y(mRaw, y, LV_PART_MAIN);
}

void LvObject::setTransformPivotX(int32_t pivot) {
    lv_obj_set_style_transform_pivot_x(mRaw, pivot, LV_PART_MAIN);
}

void LvObject::setTransformPivotY(int32_t pivot) {
    lv_obj_set_style_transform_pivot_y(mRaw, pivot, LV_PART_MAIN);
}

void LvObject::setTransformRotation(int32_t rotation) {
    lv_obj_set_style_transform_rotation(mRaw, rotation, LV_PART_MAIN);
}

void LvObject::setTransformScale(int32_t scale) {
    lv_obj_set_style_transform_scale(mRaw, scale, LV_PART_MAIN);
}

void LvObject::setTransformScale(int32_t x, int32_t y) {
    lv_obj_set_style_transform_scale_x(mRaw, x, LV_PART_MAIN);
    lv_obj_set_style_transform_scale_y(mRaw, y, LV_PART_MAIN);
}

void LvObject::setTransformScaleX(int32_t scaleX) {
    lv_obj_set_style_transform_scale_x(mRaw, scaleX, LV_PART_MAIN);
}

void LvObject::setTransformScaleY(int32_t scaleY) {
    lv_obj_set_style_transform_scale_y(mRaw, scaleY, LV_PART_MAIN);
}

void LvObject::setTransformSkew(int32_t x, int32_t y) {
    lv_obj_set_style_transform_skew_x(mRaw, x, LV_PART_MAIN);
    lv_obj_set_style_transform_skew_y(mRaw, y, LV_PART_MAIN);
}

void LvObject::setTransformSkewX(int32_t skewX) {
    lv_obj_set_style_transform_skew_x(mRaw, skewX, LV_PART_MAIN);
}

void LvObject::setTransformSkewY(int32_t skewY) {
    lv_obj_set_style_transform_skew_y(mRaw, skewY, LV_PART_MAIN);
}

// ==============================================================================
// text helpers
// ==============================================================================

void LvObject::setFont(const lv_font_t* font){
    lv_obj_set_style_text_font(mRaw, font, LV_PART_MAIN);
}

void LvObject::setTextColor(lv_color_t color){
    lv_obj_set_style_text_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setTextLetterSpace(int32_t spacePixels){
    lv_obj_set_style_text_letter_space(mRaw, spacePixels, LV_PART_MAIN);
}

void LvObject::setTextLineSpace(int32_t spacePixels){
    lv_obj_set_style_text_line_space(mRaw, spacePixels, LV_PART_MAIN);
}

void LvObject::setTextAlign(lv_text_align_t align) {
    lv_obj_set_style_text_align(mRaw, align, LV_PART_MAIN);
}

void LvObject::setTextDecor(lv_text_decor_t decor) {
    lv_obj_set_style_text_decor(mRaw, decor, LV_PART_MAIN);
}

void LvObject::setTextOpa(lv_opa_t opacity) {
    lv_obj_set_style_text_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setTextOutlineColor(lv_color_t color) {
    lv_obj_set_style_text_outline_stroke_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setTextOutlineOpa(lv_opa_t opacity) {
    lv_obj_set_style_text_outline_stroke_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setTextOutlineWidth(uint16_t width) {
    lv_obj_set_style_text_outline_stroke_width(mRaw, width, LV_PART_MAIN);
}

// ==============================================================================
// style helpers
// ==============================================================================

void LvObject::setOpa(uint8_t opacity){
    lv_obj_set_style_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setBgColor(lv_color_t color){
    lv_obj_set_style_bg_color(mRaw, color, LV_PART_MAIN);
}

void LvObject::setBgOpa(uint8_t opacity) {
    lv_obj_set_style_bg_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setRadius(int32_t radiusPixels){
    lv_obj_set_style_radius(mRaw, radiusPixels, LV_PART_MAIN);
}

// ==============================================================================
// border
// ==============================================================================

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

void LvObject::setBorderOpa(lv_opa_t opacity){
    lv_obj_set_style_border_opa(mRaw, opacity, LV_PART_MAIN);
}

void LvObject::setBorderSide(lv_border_side_t side){
    lv_obj_set_style_border_side(mRaw, side, LV_PART_MAIN);
}

void LvObject::setBorderSide(int side){
    lv_obj_set_style_border_side(mRaw, static_cast<lv_border_side_t>(side), LV_PART_MAIN);
}

void LvObject::setBorderPost(bool enabled){
    lv_obj_set_style_border_post(mRaw, enabled, LV_PART_MAIN);
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

void LvObject::setPadAll(int32_t px) {
    lv_obj_set_style_pad_all(mRaw, px, LV_PART_MAIN);
}

void LvObject::setPadHor(int32_t px) {
    lv_obj_set_style_pad_hor(mRaw, px, LV_PART_MAIN);
}

void LvObject::setPadVer(int32_t px) {
    lv_obj_set_style_pad_ver(mRaw, px, LV_PART_MAIN);
}

void LvObject::setPadGap(int32_t px) {
    lv_obj_set_style_pad_gap(mRaw, px, LV_PART_MAIN);
}

void LvObject::setPadRow(int32_t px) {
    lv_obj_set_style_pad_row(mRaw, px, LV_PART_MAIN);
}

void LvObject::setPadColumn(int32_t px) {
    lv_obj_set_style_pad_column(mRaw, px, LV_PART_MAIN);
}

void LvObject::setPadRadial(int32_t px) {
    lv_obj_set_style_pad_radial(mRaw, px, LV_PART_MAIN);
}

// ==============================================================================
// margin
// ==============================================================================

void LvObject::setMargin(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels) {
    lv_obj_set_style_margin_left(mRaw, leftPixels, LV_PART_MAIN);
    lv_obj_set_style_margin_right(mRaw, rightPixels, LV_PART_MAIN);
    lv_obj_set_style_margin_top(mRaw, topPixels, LV_PART_MAIN);
    lv_obj_set_style_margin_bottom(mRaw, bottomPixels, LV_PART_MAIN);
}

void LvObject::setMarginLeft(int32_t px) {
    lv_obj_set_style_margin_left(mRaw, px, LV_PART_MAIN);
}

void LvObject::setMarginRight(int32_t px) {
    lv_obj_set_style_margin_right(mRaw, px, LV_PART_MAIN);
}

void LvObject::setMarginTop(int32_t px) {
    lv_obj_set_style_margin_top(mRaw, px, LV_PART_MAIN);
}

void LvObject::setMarginBottom(int32_t px) {
    lv_obj_set_style_margin_bottom(mRaw, px, LV_PART_MAIN);
}

void LvObject::setMarginAll(int32_t px) {
    lv_obj_set_style_margin_all(mRaw, px, LV_PART_MAIN);
}

void LvObject::setMarginHor(int32_t px) {
    lv_obj_set_style_margin_hor(mRaw, px, LV_PART_MAIN);
}

void LvObject::setMarginVer(int32_t px) {
    lv_obj_set_style_margin_ver(mRaw, px, LV_PART_MAIN);
}

// ==============================================================================
// generic style setters
// ==============================================================================

void LvObject::setStyleProp(uint32_t prop, lv_style_value_t value){
    lv_obj_set_local_style_prop(mRaw, prop, value, LV_PART_MAIN);
}

// size / position

void LvObject::setStyleSize(int32_t width, int32_t height, lv_part_t part) {
    lv_obj_set_style_width(mRaw, width, part);
    lv_obj_set_style_height(mRaw, height, part);
}

void LvObject::setStyleWidth(int32_t width, lv_part_t part) {
    lv_obj_set_style_width(mRaw, width, part);
}

void LvObject::setStyleMinWidth(int32_t width, lv_part_t part) {
    lv_obj_set_style_min_width(mRaw, width, part);
}

void LvObject::setStyleMaxWidth(int32_t width, lv_part_t part) {
    lv_obj_set_style_max_width(mRaw, width, part);
}

void LvObject::setStyleHeight(int32_t height, lv_part_t part) {
    lv_obj_set_style_height(mRaw, height, part);
}

void LvObject::setStyleMinHeight(int32_t height, lv_part_t part) {
    lv_obj_set_style_min_height(mRaw, height, part);
}

void LvObject::setStyleMaxHeight(int32_t height, lv_part_t part) {
    lv_obj_set_style_max_height(mRaw, height, part);
}

void LvObject::setStyleX(int32_t x, lv_part_t part) {
    lv_obj_set_style_x(mRaw, x, part);
}

void LvObject::setStyleY(int32_t y, lv_part_t part) {
    lv_obj_set_style_y(mRaw, y, part);
}

void LvObject::setStyleAlign(lv_align_t align, lv_part_t part) {
    lv_obj_set_style_align(mRaw, align, part);
}

// transforms / translate

void LvObject::setStyleTransformWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_transform_width(mRaw, value, part);
}

void LvObject::setStyleTransformHeight(int32_t value, lv_part_t part) {
    lv_obj_set_style_transform_height(mRaw, value, part);
}

void LvObject::setStyleTranslateX(int32_t value, lv_part_t part) {
    lv_obj_set_style_translate_x(mRaw, value, part);
}

void LvObject::setStyleTranslateY(int32_t value, lv_part_t part) {
    lv_obj_set_style_translate_y(mRaw, value, part);
}

void LvObject::setStyleTranslateRadial(int32_t value, lv_part_t part) {
    lv_obj_set_style_translate_radial(mRaw, value, part);
}

void LvObject::setStyleTransformScaleX(int32_t value, lv_part_t part) {
    lv_obj_set_style_transform_scale_x(mRaw, value, part);
}

void LvObject::setStyleTransformScaleY(int32_t value, lv_part_t part) {
    lv_obj_set_style_transform_scale_y(mRaw, value, part);
}

void LvObject::setStyleTransformRotation(int32_t angle10deg, lv_part_t part) {
    lv_obj_set_style_transform_rotation(mRaw, angle10deg, part);
}

void LvObject::setStyleTransformPivotX(int32_t value, lv_part_t part) {
    lv_obj_set_style_transform_pivot_x(mRaw, value, part);
}

void LvObject::setStyleTransformPivotY(int32_t value, lv_part_t part) {
    lv_obj_set_style_transform_pivot_y(mRaw, value, part);
}

void LvObject::setStyleTransformSkewX(int32_t angle10deg, lv_part_t part) {
    lv_obj_set_style_transform_skew_x(mRaw, angle10deg, part);
}

void LvObject::setStyleTransformSkewY(int32_t angle10deg, lv_part_t part) {
    lv_obj_set_style_transform_skew_y(mRaw, angle10deg, part);
}

// padding / margin

void LvObject::setStylePadTop(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_top(mRaw, value, part);
}

void LvObject::setStylePadBottom(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_bottom(mRaw, value, part);
}

void LvObject::setStylePadLeft(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_left(mRaw, value, part);
}

void LvObject::setStylePadRight(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_right(mRaw, value, part);
}

void LvObject::setStylePadRow(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_row(mRaw, value, part);
}

void LvObject::setStylePadColumn(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_column(mRaw, value, part);
}

void LvObject::setStylePadRadial(int32_t value, lv_part_t part) {
    lv_obj_set_style_pad_radial(mRaw, value, part);
}

void LvObject::setStyleMarginTop(int32_t value, lv_part_t part) {
    lv_obj_set_style_margin_top(mRaw, value, part);
}

void LvObject::setStyleMarginBottom(int32_t value, lv_part_t part) {
    lv_obj_set_style_margin_bottom(mRaw, value, part);
}

void LvObject::setStyleMarginLeft(int32_t value, lv_part_t part) {
    lv_obj_set_style_margin_left(mRaw, value, part);
}

void LvObject::setStyleMarginRight(int32_t value, lv_part_t part) {
    lv_obj_set_style_margin_right(mRaw, value, part);
}

// background

void LvObject::setStyleBgColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_bg_color(mRaw, value, part);
}

void LvObject::setStyleBgOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_bg_opa(mRaw, value, part);
}

void LvObject::setStyleBgGradColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_bg_grad_color(mRaw, value, part);
}

void LvObject::setStyleBgGradDir(lv_grad_dir_t dir, lv_part_t part) {
    lv_obj_set_style_bg_grad_dir(mRaw, dir, part);
}

void LvObject::setStyleBgMainStop(int32_t value, lv_part_t part) {
    lv_obj_set_style_bg_main_stop(mRaw, value, part);
}

void LvObject::setStyleBgGradStop(int32_t value, lv_part_t part) {
    lv_obj_set_style_bg_grad_stop(mRaw, value, part);
}

void LvObject::setStyleBgMainOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_bg_main_opa(mRaw, value, part);
}

void LvObject::setStyleBgGradOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_bg_grad_opa(mRaw, value, part);
}

void LvObject::setStyleBgGrad(const lv_grad_dsc_t* grad, lv_part_t part) {
    lv_obj_set_style_bg_grad(mRaw, grad, part);
}

void LvObject::setStyleBgImageSrc(const void* src, lv_part_t part) {
    lv_obj_set_style_bg_image_src(mRaw, src, part);
}

void LvObject::setStyleBgImageOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_bg_image_opa(mRaw, value, part);
}

void LvObject::setStyleBgImageRecolor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_bg_image_recolor(mRaw, value, part);
}

void LvObject::setStyleBgImageRecolorOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_bg_image_recolor_opa(mRaw, value, part);
}

void LvObject::setStyleBgImageTiled(bool tiled, lv_part_t part) {
    lv_obj_set_style_bg_image_tiled(mRaw, tiled, part);
}

// border / outline

void LvObject::setStyleBorderColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_border_color(mRaw, value, part);
}

void LvObject::setStyleBorderOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_border_opa(mRaw, value, part);
}

void LvObject::setStyleBorderWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_border_width(mRaw, value, part);
}

void LvObject::setStyleBorderSide(lv_border_side_t value, lv_part_t part) {
    lv_obj_set_style_border_side(mRaw, value, part);
}

void LvObject::setStyleBorderPost(bool value, lv_part_t part) {
    lv_obj_set_style_border_post(mRaw, value, part);
}

void LvObject::setStyleOutlineWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_outline_width(mRaw, value, part);
}

void LvObject::setStyleOutlineColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_outline_color(mRaw, value, part);
}

void LvObject::setStyleOutlineOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_outline_opa(mRaw, value, part);
}

void LvObject::setStyleOutlinePad(int32_t value, lv_part_t part) {
    lv_obj_set_style_outline_pad(mRaw, value, part);
}

// shadow

void LvObject::setStyleShadowWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_shadow_width(mRaw, value, part);
}

void LvObject::setStyleShadowOffsetX(int32_t value, lv_part_t part) {
    lv_obj_set_style_shadow_offset_x(mRaw, value, part);
}

void LvObject::setStyleShadowOffsetY(int32_t value, lv_part_t part) {
    lv_obj_set_style_shadow_offset_y(mRaw, value, part);
}

void LvObject::setStyleShadowSpread(int32_t value, lv_part_t part) {
    lv_obj_set_style_shadow_spread(mRaw, value, part);
}

void LvObject::setStyleShadowColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_shadow_color(mRaw, value, part);
}

void LvObject::setStyleShadowOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_shadow_opa(mRaw, value, part);
}

// image style

void LvObject::setStyleImageOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_image_opa(mRaw, value, part);
}

void LvObject::setStyleImageRecolor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_image_recolor(mRaw, value, part);
}

void LvObject::setStyleImageRecolorOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_image_recolor_opa(mRaw, value, part);
}

void LvObject::setStyleImageColorkey(const lv_image_colorkey_t* key, lv_part_t part) {
    lv_obj_set_style_image_colorkey(mRaw, key, part);
}

// line / arc

void LvObject::setStyleLineWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_line_width(mRaw, value, part);
}

void LvObject::setStyleLineDashWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_line_dash_width(mRaw, value, part);
}

void LvObject::setStyleLineDashGap(int32_t value, lv_part_t part) {
    lv_obj_set_style_line_dash_gap(mRaw, value, part);
}

void LvObject::setStyleLineRounded(bool value, lv_part_t part) {
    lv_obj_set_style_line_rounded(mRaw, value, part);
}

void LvObject::setStyleLineColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_line_color(mRaw, value, part);
}

void LvObject::setStyleLineOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_line_opa(mRaw, value, part);
}

void LvObject::setStyleArcWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_arc_width(mRaw, value, part);
}

void LvObject::setStyleArcRounded(bool value, lv_part_t part) {
    lv_obj_set_style_arc_rounded(mRaw, value, part);
}

void LvObject::setStyleArcColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_arc_color(mRaw, value, part);
}

void LvObject::setStyleArcOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_arc_opa(mRaw, value, part);
}

void LvObject::setStyleArcImageSrc(const void* src, lv_part_t part) {
    lv_obj_set_style_arc_image_src(mRaw, src, part);
}

// text

void LvObject::setStyleTextColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_text_color(mRaw, value, part);
}

void LvObject::setStyleTextOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_text_opa(mRaw, value, part);
}

void LvObject::setStyleTextFont(const lv_font_t* font, lv_part_t part) {
    lv_obj_set_style_text_font(mRaw, font, part);
}

void LvObject::setStyleTextLetterSpace(int32_t value, lv_part_t part) {
    lv_obj_set_style_text_letter_space(mRaw, value, part);
}

void LvObject::setStyleTextLineSpace(int32_t value, lv_part_t part) {
    lv_obj_set_style_text_line_space(mRaw, value, part);
}

void LvObject::setStyleTextDecor(lv_text_decor_t value, lv_part_t part) {
    lv_obj_set_style_text_decor(mRaw, value, part);
}

void LvObject::setStyleTextAlign(lv_text_align_t value, lv_part_t part) {
    lv_obj_set_style_text_align(mRaw, value, part);
}

void LvObject::setStyleTextOutlineColor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_text_outline_stroke_color(mRaw, value, part);
}

void LvObject::setStyleTextOutlineWidth(int32_t value, lv_part_t part) {
    lv_obj_set_style_text_outline_stroke_width(mRaw, value, part);
}

void LvObject::setStyleTextOutlineOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_text_outline_stroke_opa(mRaw, value, part);
}

// misc

void LvObject::setStyleRadius(int32_t value, lv_part_t part) {
    lv_obj_set_style_radius(mRaw, value, part);
}

void LvObject::setStyleRadialOffset(int32_t value, lv_part_t part) {
    lv_obj_set_style_radial_offset(mRaw, value, part);
}

void LvObject::setStyleClipCorner(bool value, lv_part_t part) {
    lv_obj_set_style_clip_corner(mRaw, value, part);
}

void LvObject::setStyleOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_opa(mRaw, value, part);
}

void LvObject::setStyleOpaLayered(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_opa_layered(mRaw, value, part);
}

void LvObject::setStyleColorFilterDsc(const lv_color_filter_dsc_t* dsc, lv_part_t part) {
    lv_obj_set_style_color_filter_dsc(mRaw, dsc, part);
}

void LvObject::setStyleColorFilterOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_color_filter_opa(mRaw, value, part);
}

void LvObject::setStyleRecolor(lv_color_t value, lv_part_t part) {
    lv_obj_set_style_recolor(mRaw, value, part);
}

void LvObject::setStyleRecolorOpa(lv_opa_t value, lv_part_t part) {
    lv_obj_set_style_recolor_opa(mRaw, value, part);
}

void LvObject::setStyleBlendMode(lv_blend_mode_t mode, lv_part_t part) {
    lv_obj_set_style_blend_mode(mRaw, mode, part);
}

void LvObject::setStyleLayout(uint16_t layout, lv_part_t part) {
    lv_obj_set_style_layout(mRaw, layout, part);
}

void LvObject::setStyleBaseDir(lv_base_dir_t dir, lv_part_t part) {
    lv_obj_set_style_base_dir(mRaw, dir, part);
}

void LvObject::setStyleBitmapMaskSrc(const void* src, lv_part_t part) {
    lv_obj_set_style_bitmap_mask_src(mRaw, src, part);
}

void LvObject::setStyleRotarySensitivity(uint32_t value, lv_part_t part) {
    lv_obj_set_style_rotary_sensitivity(mRaw, value, part);
}

// flex / grid

void LvObject::setStyleFlexFlow(lv_flex_flow_t flow, lv_part_t part) {
    lv_obj_set_style_flex_flow(mRaw, flow, part);
}

void LvObject::setStyleFlexMainPlace(lv_flex_align_t value, lv_part_t part) {
    lv_obj_set_style_flex_main_place(mRaw, value, part);
}

void LvObject::setStyleFlexCrossPlace(lv_flex_align_t value, lv_part_t part) {
    lv_obj_set_style_flex_cross_place(mRaw, value, part);
}

void LvObject::setStyleFlexTrackPlace(lv_flex_align_t value, lv_part_t part) {
    lv_obj_set_style_flex_track_place(mRaw, value, part);
}

void LvObject::setStyleFlexGrow(uint8_t value, lv_part_t part) {
    lv_obj_set_style_flex_grow(mRaw, value, part);
}

void LvObject::setStyleGridColumnDscArray(const int32_t* value, lv_part_t part) {
    lv_obj_set_style_grid_column_dsc_array(mRaw, value, part);
}

void LvObject::setStyleGridColumnAlign(lv_grid_align_t value, lv_part_t part) {
    lv_obj_set_style_grid_column_align(mRaw, value, part);
}

void LvObject::setStyleGridRowDscArray(const int32_t* value, lv_part_t part) {
    lv_obj_set_style_grid_row_dsc_array(mRaw, value, part);
}

void LvObject::setStyleGridRowAlign(lv_grid_align_t value, lv_part_t part) {
    lv_obj_set_style_grid_row_align(mRaw, value, part);
}

void LvObject::setStyleGridCellColumnPos(int32_t value, lv_part_t part) {
    lv_obj_set_style_grid_cell_column_pos(mRaw, value, part);
}

void LvObject::setStyleGridCellXAlign(lv_grid_align_t value, lv_part_t part) {
    lv_obj_set_style_grid_cell_x_align(mRaw, value, part);
}

void LvObject::setStyleGridCellColumnSpan(int32_t value, lv_part_t part) {
    lv_obj_set_style_grid_cell_column_span(mRaw, value, part);
}

void LvObject::setStyleGridCellRowPos(int32_t value, lv_part_t part) {
    lv_obj_set_style_grid_cell_row_pos(mRaw, value, part);
}

void LvObject::setStyleGridCellYAlign(lv_grid_align_t value, lv_part_t part) {
    lv_obj_set_style_grid_cell_y_align(mRaw, value, part);
}

void LvObject::setStyleGridCellRowSpan(int32_t value, lv_part_t part) {
    lv_obj_set_style_grid_cell_row_span(mRaw, value, part);
}

// ==============================================================================
// generic style setters
// ==============================================================================

void LvObject::addEventCallback(lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data){
    lv_obj_add_event_cb(mRaw, event_cb, filter, user_data);
}

} // namespace Garbox
