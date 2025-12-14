#pragma once

#include <lvgl.h>
#include "core/time/Time.h"

namespace Garbox {

/**
 * Non-owning wrapper around a LVGL object.
 * 
 * Owns the reference to the underlying LVGL object.
 */
class LvObject {
protected:

public:

    struct Size {
        int32_t width;
        int32_t height;
    };

    // create a 
    LvObject();

    // create an object with parent
    LvObject(LvObject& parent);
    LvObject(lv_obj_t* raw);

    // does not free the lvgl object
    ~LvObject();

    // wrapper can not be copied
    LvObject(const LvObject& other) = delete; // copy constructor
    LvObject& operator=(const LvObject& other) = delete; // assignment operator
    
    // wrapper can be moved
    LvObject(LvObject&& other) noexcept; // move constructor
    LvObject& operator=(LvObject&& other) noexcept; // move assignment operator

    // raw lvgl object access
    lv_obj_t* raw() const;

    // deletes the lvgl object
    void free();

    // set this object as the main screen
    void setScreen();

    // ordering
    void moveBackground();
    void moveForeground();
    void moveToIndex(int32_t index);
    int32_t getIndex();

    // raw positioning
    void setPosition(int32_t xPixels, int32_t yPixels);
    void setPositionX(int32_t xPixels);
    void setPositionY(int32_t yPixels);

    // position getters
    void getPosition(int32_t& xPixels, int32_t& yPixels) const;
    int32_t getPositionX() const;
    int32_t getPositionY() const;

    // width and height getters
    int32_t getRawWidth() const;
    int32_t getRawHeight() const; 
    int32_t getWidth() const;
    int32_t getHeight() const;  
    int32_t getContentWidth() const;
    int32_t getContentHeight() const; 

    // size getters
    void getRawSize(int32_t& widthPixels, int32_t& heightPixels) const;
    void getSize(int32_t& widthPixels, int32_t& heightPixels) const;

    // coords getters 
    void getCoords(lv_area_t& area) const;
    void getContentCoords(lv_area_t& area) const;

    // raw sizing 
    void setRawSize(int32_t widthPixels, int32_t heightPixels);
    void setRawWidth(int32_t widthPixels);
    void setRawHeight(int32_t heightPixels);

    // style based sizing
    void setSize(lv_coord_t width, lv_coord_t height);
    void setWidth(lv_coord_t value);
    void setHeight(lv_coord_t value);
    
    // alignment helpers
    void center();
    void setAlign(lv_align_t alignment, int32_t xOffset = 0, int32_t yOffset = 0);

    // content sizing helpers
    void setRawWidthContent();
    void setRawHeightContent();
    void setRawSizeContent();
    void setWidthContent();
    void setHeightContent();
    void setSizeContent();

    // layout
    void setLayout(lv_layout_t layout);

    // flex
    void setFlexFlow(lv_flex_flow_t flow);
    void setFlexGrow(uint8_t grow);
    void setFlexAlign(lv_flex_align_t main_place, lv_flex_align_t cross_place, lv_flex_align_t track_place);

    // grid
    void setGridColumns(const lv_coord_t* columns);
    void setGridRows(const lv_coord_t* rows);
    void setGridCell(lv_coord_t col, lv_coord_t row, lv_coord_t col_span, lv_coord_t row_span);

    // flags
    void addFlag(lv_obj_flag_t flag);
    void clearFlag(lv_obj_flag_t flag);
    void setHidden(bool hidden);

    // scrolling
    void setScrollable(bool scrollable);
    void setScrollOne(bool enabled);
    void setScrollChain(bool enabled);
    void setScrollMomentum(bool enabled);
    void setScrollElastic(bool enabled);
    void setScrollDirection(lv_dir_t dir);

    // transform
    void setTransformWidth(int32_t width);
    void setTransformHeight(int32_t height);
    void setTransformPivot(int32_t x, int32_t y);
    void setTransformPivotX(int32_t pivot);
    void setTransformPivotY(int32_t pivot);
    void setTransformRotation(int32_t rotation);
    void setTransformScale(int32_t scale);
    void setTransformScale(int32_t scaleX, int32_t scaleY);
    void setTransformScaleX(int32_t scaleX);
    void setTransformScaleY(int32_t scaleY);
    void setTransformSkew(int32_t skewX, int32_t skewY);
    void setTransformSkewX(int32_t skewX);
    void setTransformSkewY(int32_t skewY);

    // text helpers
    void setFont(const lv_font_t* font);
    void setTextColor(lv_color_t color);
    void setTextLetterSpace(int32_t spacePixels);
    void setTextLineSpace(int32_t spacePixels);
    void setTextAlign(lv_text_align_t align);
    void setTextDecor(lv_text_decor_t decor);
    void setTextOpa(lv_opa_t opacity);
    void setTextOutlineColor(lv_color_t color);
    void setTextOutlineOpa(lv_opa_t opacity);
    void setTextOutlineWidth(uint16_t width);

    // style helpers    
    void setOpa(uint8_t opacity);
    void setBgColor(lv_color_t color);
    void setBgOpa(uint8_t opacity);
    void setRadius(int32_t radiusPixels);

    // border
    void setBorder(int32_t widthPixels, lv_color_t color);
    void setBorderWidth(int32_t widthPixels);
    void setBorderColor(lv_color_t color);
    void setBorderOpa(lv_opa_t opacity);
    void setBorderSide(lv_border_side_t side);
    void setBorderSide(int side);
    void setBorderPost(bool enabled);

    // padding
    void setPad(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels);
    void setPadLeft(int32_t leftPixels);
    void setPadRight(int32_t rightPixels);
    void setPadTop(int32_t topPixels);
    void setPadBottom(int32_t bottomPixels);
    void setPadAll(int32_t px);
    void setPadHor(int32_t px);
    void setPadVer(int32_t px);
    void setPadGap(int32_t px);
    void setPadRow(int32_t px);
    void setPadColumn(int32_t px);
    void setPadRadial(int32_t px);

    // margin
    void setMargin(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels);
    void setMarginLeft(int32_t px);
    void setMarginRight(int32_t px);
    void setMarginTop(int32_t px);
    void setMarginBottom(int32_t px);
    void setMarginAll(int32_t px);
    void setMarginHor(int32_t px);
    void setMarginVer(int32_t px);

    // ===========================================================================
    // generic styles
    // ===========================================================================

    void setStyleProp(uint32_t prop, lv_style_value_t value);   

    // size / position
    void setStyleSize(int32_t width, int32_t height, lv_part_t part);
    void setStyleWidth(int32_t width, lv_part_t part);
    void setStyleMinWidth(int32_t width, lv_part_t part);
    void setStyleMaxWidth(int32_t width, lv_part_t part);
    void setStyleHeight(int32_t height, lv_part_t part);
    void setStyleMinHeight(int32_t height, lv_part_t part);
    void setStyleMaxHeight(int32_t height, lv_part_t part);
    void setStyleX(int32_t x, lv_part_t part);
    void setStyleY(int32_t y, lv_part_t part);
    void setStyleAlign(lv_align_t align, lv_part_t part);

    // transforms / translate
    void setStyleTransformWidth(int32_t value, lv_part_t part);
    void setStyleTransformHeight(int32_t value, lv_part_t part);
    void setStyleTranslateX(int32_t value, lv_part_t part);
    void setStyleTranslateY(int32_t value, lv_part_t part);
    void setStyleTranslateRadial(int32_t value, lv_part_t part);
    void setStyleTransformScaleX(int32_t value, lv_part_t part);
    void setStyleTransformScaleY(int32_t value, lv_part_t part);
    void setStyleTransformRotation(int32_t angle10deg, lv_part_t part);
    void setStyleTransformPivotX(int32_t value, lv_part_t part);
    void setStyleTransformPivotY(int32_t value, lv_part_t part);
    void setStyleTransformSkewX(int32_t angle10deg, lv_part_t part);
    void setStyleTransformSkewY(int32_t angle10deg, lv_part_t part);

    // padding / margin
    void setStylePadTop(int32_t value, lv_part_t part);
    void setStylePadBottom(int32_t value, lv_part_t part);
    void setStylePadLeft(int32_t value, lv_part_t part);
    void setStylePadRight(int32_t value, lv_part_t part);
    void setStylePadRow(int32_t value, lv_part_t part);
    void setStylePadColumn(int32_t value, lv_part_t part);
    void setStylePadRadial(int32_t value, lv_part_t part);

    void setStyleMarginTop(int32_t value, lv_part_t part);
    void setStyleMarginBottom(int32_t value, lv_part_t part);
    void setStyleMarginLeft(int32_t value, lv_part_t part);
    void setStyleMarginRight(int32_t value, lv_part_t part);

    // background
    void setStyleBgColor(lv_color_t value, lv_part_t part);
    void setStyleBgOpa(lv_opa_t value, lv_part_t part);
    void setStyleBgGradColor(lv_color_t value, lv_part_t part);
    void setStyleBgGradDir(lv_grad_dir_t dir, lv_part_t part);
    void setStyleBgMainStop(int32_t value, lv_part_t part);
    void setStyleBgGradStop(int32_t value, lv_part_t part);
    void setStyleBgMainOpa(lv_opa_t value, lv_part_t part);
    void setStyleBgGradOpa(lv_opa_t value, lv_part_t part);
    void setStyleBgGrad(const lv_grad_dsc_t* grad, lv_part_t part);
    void setStyleBgImageSrc(const void* src, lv_part_t part);
    void setStyleBgImageOpa(lv_opa_t value, lv_part_t part);
    void setStyleBgImageRecolor(lv_color_t value, lv_part_t part);
    void setStyleBgImageRecolorOpa(lv_opa_t value, lv_part_t part);
    void setStyleBgImageTiled(bool tiled, lv_part_t part);

    // border / outline
    void setStyleBorderColor(lv_color_t value, lv_part_t part);
    void setStyleBorderOpa(lv_opa_t value, lv_part_t part);
    void setStyleBorderWidth(int32_t value, lv_part_t part);
    void setStyleBorderSide(lv_border_side_t value, lv_part_t part);
    void setStyleBorderPost(bool value, lv_part_t part);

    void setStyleOutlineWidth(int32_t value, lv_part_t part);
    void setStyleOutlineColor(lv_color_t value, lv_part_t part);
    void setStyleOutlineOpa(lv_opa_t value, lv_part_t part);
    void setStyleOutlinePad(int32_t value, lv_part_t part);

    // shadow
    void setStyleShadowWidth(int32_t value, lv_part_t part);
    void setStyleShadowOffsetX(int32_t value, lv_part_t part);
    void setStyleShadowOffsetY(int32_t value, lv_part_t part);
    void setStyleShadowSpread(int32_t value, lv_part_t part);
    void setStyleShadowColor(lv_color_t value, lv_part_t part);
    void setStyleShadowOpa(lv_opa_t value, lv_part_t part);

    // image style
    void setStyleImageOpa(lv_opa_t value, lv_part_t part);
    void setStyleImageRecolor(lv_color_t value, lv_part_t part);
    void setStyleImageRecolorOpa(lv_opa_t value, lv_part_t part);
    void setStyleImageColorkey(const lv_image_colorkey_t* key, lv_part_t part);

    // line / arc
    void setStyleLineWidth(int32_t value, lv_part_t part);
    void setStyleLineDashWidth(int32_t value, lv_part_t part);
    void setStyleLineDashGap(int32_t value, lv_part_t part);
    void setStyleLineRounded(bool value, lv_part_t part);
    void setStyleLineColor(lv_color_t value, lv_part_t part);
    void setStyleLineOpa(lv_opa_t value, lv_part_t part);

    void setStyleArcWidth(int32_t value, lv_part_t part);
    void setStyleArcRounded(bool value, lv_part_t part);
    void setStyleArcColor(lv_color_t value, lv_part_t part);
    void setStyleArcOpa(lv_opa_t value, lv_part_t part);
    void setStyleArcImageSrc(const void* src, lv_part_t part);

    // text
    void setStyleTextColor(lv_color_t value, lv_part_t part);
    void setStyleTextOpa(lv_opa_t value, lv_part_t part);
    void setStyleTextFont(const lv_font_t* font, lv_part_t part);
    void setStyleTextLetterSpace(int32_t value, lv_part_t part);
    void setStyleTextLineSpace(int32_t value, lv_part_t part);
    void setStyleTextDecor(lv_text_decor_t value, lv_part_t part);
    void setStyleTextAlign(lv_text_align_t value, lv_part_t part);
    void setStyleTextOutlineColor(lv_color_t value, lv_part_t part);
    void setStyleTextOutlineWidth(int32_t value, lv_part_t part);
    void setStyleTextOutlineOpa(lv_opa_t value, lv_part_t part);

    // misc
    void setStyleRadius(int32_t value, lv_part_t part);
    void setStyleRadialOffset(int32_t value, lv_part_t part);
    void setStyleClipCorner(bool value, lv_part_t part);
    void setStyleOpa(lv_opa_t value, lv_part_t part);
    void setStyleOpaLayered(lv_opa_t value, lv_part_t part);
    void setStyleColorFilterDsc(const lv_color_filter_dsc_t* dsc, lv_part_t part);
    void setStyleColorFilterOpa(lv_opa_t value, lv_part_t part);
    void setStyleRecolor(lv_color_t value, lv_part_t part);
    void setStyleRecolorOpa(lv_opa_t value, lv_part_t part);
    void setStyleBlendMode(lv_blend_mode_t mode, lv_part_t part);
    void setStyleLayout(uint16_t layout, lv_part_t part);
    void setStyleBaseDir(lv_base_dir_t dir, lv_part_t part);
    void setStyleBitmapMaskSrc(const void* src, lv_part_t part);
    void setStyleRotarySensitivity(uint32_t value, lv_part_t part);

    // flex / grid
    void setStyleFlexFlow(lv_flex_flow_t flow, lv_part_t part);
    void setStyleFlexMainPlace(lv_flex_align_t value, lv_part_t part);
    void setStyleFlexCrossPlace(lv_flex_align_t value, lv_part_t part);
    void setStyleFlexTrackPlace(lv_flex_align_t value, lv_part_t part);
    void setStyleFlexGrow(uint8_t value, lv_part_t part);

    void setStyleGridColumnDscArray(const int32_t* value, lv_part_t part);
    void setStyleGridColumnAlign(lv_grid_align_t value, lv_part_t part);
    void setStyleGridRowDscArray(const int32_t* value, lv_part_t part);
    void setStyleGridRowAlign(lv_grid_align_t value, lv_part_t part);
    void setStyleGridCellColumnPos(int32_t value, lv_part_t part);
    void setStyleGridCellXAlign(lv_grid_align_t value, lv_part_t part);
    void setStyleGridCellColumnSpan(int32_t value, lv_part_t part);
    void setStyleGridCellRowPos(int32_t value, lv_part_t part);
    void setStyleGridCellYAlign(lv_grid_align_t value, lv_part_t part);
    void setStyleGridCellRowSpan(int32_t value, lv_part_t part);

    // callbacks
    void addEventCallback(lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data);

protected:
    
    // raw pointer to lvgl object 
    lv_obj_t* mRaw = nullptr;

};

} // namespace Garbox
