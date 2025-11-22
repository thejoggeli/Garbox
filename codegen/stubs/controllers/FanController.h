// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/FanControllerAbs.h"

namespace Garbox {

class FanController : public FanControllerAbs {
public:

    FanController();

    void onInputTick() final;
    void onOutputTick() final;

    void onFanCommand(const FanCommand& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox