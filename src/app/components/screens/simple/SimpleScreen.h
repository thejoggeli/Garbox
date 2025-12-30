#pragma once
// Sections marked with GENERATED BEGIN/END may be overwritten by the generator.
// Do not modify or remove these sections. Changes outside these markers will be preserved.

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/screens/simple/SimpleScreenAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/lvgl/objects/LvArc.h"

namespace Garbox {

class SimpleScreen : public SimpleScreenAbs {
private:

    void initArc(LvArc& arc);

public:
// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    SimpleScreen();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated tick handlers
    void onUserInputTick() final;

    // generated render functions
    void onRender() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox