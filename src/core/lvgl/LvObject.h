#pragma once

#include <lvgl.h>
#include "core/time/Time.h"

namespace Garbox {

class LvObject {
public:

    LvObject(lv_obj_t* raw);
    
    lv_obj_t* raw() const;

    // geometry
    void setSize(int32_t widthPixels, int32_t heightPixels);
    void setPosition(int32_t xPixels, int32_t yPixels);

    // content sizing helpers
    void setWidthContent();
    void setHeightContent();
    void setSizeContent();
    void setFlexGrow(uint8_t grow);

    // flags
    void addFlag(lv_obj_flag_t flag);
    void clearFlag(lv_obj_flag_t flag);
    void setHidden(bool hidden);
    void setScrollable(bool scrollable);

    // style helpers
    void setTextColor(lv_color_t color);
    void setFont(const lv_font_t* font);
    void setOpacity(uint8_t opacity);
    void setBgColor(lv_color_t color);
    void setPad(int32_t leftPixels, int32_t rightPixels, int32_t topPixels, int32_t bottomPixels);
    void setBorder(int32_t widthPixels, lv_color_t color);
    void setRadius(int32_t radiusPixels);
    void setTextLetterSpace(int32_t spacePixels);
    void setTextLineSpace(int32_t spacePixels);

    // generic style setter
    void setStyleProp(uint32_t prop, lv_style_value_t value);

protected:
    
    // raw pointer to lvgl object 
    lv_obj_t* mRaw = nullptr;
    
    // buffer for string formatting
    static char sBuffer[256];

};

} // namespace Garbox
