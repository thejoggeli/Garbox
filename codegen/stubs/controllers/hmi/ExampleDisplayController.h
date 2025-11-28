#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/DisplayControllerAbs.h"

namespace Garbox {

class DisplayController : public DisplayControllerAbs {
public:

    DisplayController();

    void onRenderTick() final;

    void onDisplayCommand(const DisplayCommandEvent& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox