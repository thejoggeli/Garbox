// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/InputControllerAbs.h"

namespace Garbox {

class InputController : public InputControllerAbs {
public:

    InputController(const RuntimeContext& context);

    void onInputTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox