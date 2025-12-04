#include "LvContainer.h"

namespace Garbox {

LvContainer::LvContainer(LvObject& parent):
    // initialize members
    LvObject(lv_obj_create(parent.raw())){
    // constructor body
}

} // namespace Garbox
