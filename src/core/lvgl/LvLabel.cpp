#include "LvLabel.h"

#include <cstdio>
namespace Garbox {

LvLabel::LvLabel(LvObject& parent): LvObject(lv_label_create(parent.raw())){
    // constructor body
}

void LvLabel::setText(const char* text){
    lv_label_set_text(raw(), text);
}

void LvLabel::setTextFormatted(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    vsnprintf(sBuffer, sizeof(sBuffer), fmt, args);
    va_end(args);
    lv_label_set_text(mRaw, sBuffer);
}

void LvLabel::setLongMode(lv_label_long_mode_t mode){
    lv_label_set_long_mode(raw(), mode);
}

void LvLabel::setAlign(lv_text_align_t align){
    // align label text via style
    lv_obj_set_style_text_align(raw(), align, LV_PART_MAIN);
}

} // namespace Garbox
