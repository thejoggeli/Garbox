#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/HeatpadControllerAbs.h"

namespace Garbox {

class HeatpadController : public HeatpadControllerAbs {
public:

    HeatpadController();

    void onInputTick() final;
    void onOutputTick() final;

    void onHeatpadCommand(const HeatpadCommandEvent& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox