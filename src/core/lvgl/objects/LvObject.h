#pragma once

#include <lvgl.h>
#include "core/time/Time.h"

namespace Garbox {

class LvObject {
public:

    struct Size {
        int32_t width;
        int32_t height;
    };

    LvObject(lv_obj_t* raw);
    
    lv_obj_t* raw() const;

    // raw positioning
    void setPosition(int32_t xPixels, int32_t yPixels);
    void setPositionX(int32_t xPixels);
    void setPositionY(int32_t yPixels);
    void getPosition(int32_t& xPixels, int32_t& yPixels) const;
    int32_t getPositionX() const;
    int32_t getPositionY() const;

    // area
    void getArea(lv_area_t& area) const;

    // raw sizing 
    void setRawSize(int32_t widthPixels, int32_t heightPixels);
    void setRawWidth(int32_t widthPixels);
    void setRawHeight(int32_t heightPixels);
    void getRawSize(int32_t& widthPixels, int32_t& heightPixels) const;
    int32_t getRawWidth() const;
    int32_t getRawHeight() const; 

    // style based sizing
    void setSize(lv_coord_t width, lv_coord_t height);
    void setWidth(lv_coord_t value);
    void setHeight(lv_coord_t value);
    void getSize(int32_t& widthPixels, int32_t& heightPixels) const;
    int32_t getWidth() const;
    int32_t getHeight() const; 
    
    // alignment helpers
    void center();

    // content sizing helpers
    void setWidthContent();
    void setHeightContent();
    void setSizeContent();

    // layout
    void setLayout(lv_layout_t layout);

    // flex
    void setFlexFlow(lv_flex_flow_t flow);
    void setFlexGrow(uint8_t grow);

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

    // style helpers
    void setTextColor(lv_color_t color);
    void setFont(const lv_font_t* font);
    void setOpacity(uint8_t opacity);
    void setBgColor(lv_color_t color);
    void setBgOpacity(uint8_t opacity);
    void setBorder(int32_t widthPixels, lv_color_t color);
    void setBorderWidth(int32_t widthPixels);
    void setBorderColor(lv_color_t color);
    void setRadius(int32_t radiusPixels);
    void setTextLetterSpace(int32_t spacePixels);
    void setTextLineSpace(int32_t spacePixels);

    // padding
    void setPad(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels);
    void setPadLeft(int32_t leftPixels);
    void setPadRight(int32_t rightPixels);
    void setPadTop(int32_t topPixels);
    void setPadBottom(int32_t bottomPixels);
    void setPadX(int32_t xPixels);
    void setPadY(int32_t yPixels);

    // style helper
    void setStyleProp(uint32_t prop, lv_style_value_t value);   

protected:
    
    // raw pointer to lvgl object 
    lv_obj_t* mRaw = nullptr;
    
    // buffer for string formatting
    static char sBuffer[256];

};

} // namespace Garbox
