#include "LvImage.h"

namespace Garbox {

LvImage::LvImage(LvObject& parent): LvObject(lv_image_create(parent.raw())) {}

void LvImage::setSource(const void* src){
    lv_image_set_src(mRaw, src);
}

void LvImage::setPivot(lv_coord_t x, lv_coord_t y){
    lv_image_set_pivot(mRaw, x, y);
}

void LvImage::setScale(uint32_t zoom){
    lv_image_set_scale(mRaw, zoom);
}

} // namespace Garbox
