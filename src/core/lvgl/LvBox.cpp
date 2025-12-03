#include "LvBox.h"

namespace Garbox {

LvBox::LvBox(LvObject& parent) : LvObject(lv_obj_create(parent.raw())){
    // constructor body
}

} // namespace Garbox
