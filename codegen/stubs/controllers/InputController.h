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