#pragma once

#include "app/controllers/generated/I2cPartsControllerAbs.h"

namespace Garbox {

class I2cPartsController : public I2cPartsControllerAbs {
public:

    I2cPartsController(const RuntimeContext& context);

    void onInputTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox