#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/FanControllerAbs.h"

namespace Garbox {

class FanController : public FanControllerAbs {
public:

    FanController();

    // tick handlers
    void onInputTick() final;
    void onOutputTick() final;

    // event handlers
    virtual void onFanCommand(const FanCommandEvent& event) = 0;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox