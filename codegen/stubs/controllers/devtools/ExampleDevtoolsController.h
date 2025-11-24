#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/DevtoolsControllerAbs.h"

namespace Garbox {

class DevtoolsController : public DevtoolsControllerAbs {
public:

    DevtoolsController();

    void onLoggingTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox