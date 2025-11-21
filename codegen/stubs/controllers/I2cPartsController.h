// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/I2cPartsControllerAbs.h"

namespace Garbox {

class I2cPartsController : public I2cPartsControllerAbs {
public:

    I2cPartsController(const RuntimeContext& context);

    void onInputTick() final;

    void onButtonStateChanged(const EventRead<EventPayload::ButtonStateChanged>& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox