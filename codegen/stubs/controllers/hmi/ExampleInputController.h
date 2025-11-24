#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/InputControllerAbs.h"

namespace Garbox {

class InputController : public InputControllerAbs {
public:

    InputController();

    void onInputTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox