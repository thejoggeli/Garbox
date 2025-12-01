#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/MainScreenAbs.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

    // update all user interface elements in this method
    void onUpdateScreen() final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox