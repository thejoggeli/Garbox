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
    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    lv_label_set_text(mRaw, buffer);
}

void LvLabel::setLongMode(lv_label_long_mode_t mode){
    lv_label_set_long_mode(raw(), mode);
}

} // namespace Garbox
