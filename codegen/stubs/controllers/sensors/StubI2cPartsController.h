#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/I2cPartsControllerAbs.h"

namespace Garbox {

class I2cPartsController : public I2cPartsControllerAbs {
public:

    I2cPartsController();

    // tick handlers
    void onInputTick() final;

    // event handlers
    virtual void onButtonStateChanged(const ButtonStateChangedEvent& event) = 0;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox