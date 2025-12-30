#include "SimpleScreen.h"
// This file was initially generated and is no longer modified by the generator.
// Interface changes must be applied manually in this file.
// See the corresponding header for the interface method declarations.

#include <math.h>
#include "core/util/function/default/MathFunctions.h"

namespace Garbox {

SimpleScreen::SimpleScreen() : SimpleScreenAbs(){
    // to be implemented
}

void SimpleScreen::onInit(){
    gui().tempLabel.setFont(&lv_font_montserrat_12);
    gui().tempValue.setFont(&lv_font_montserrat_30);
    gui().tempUnit.setFont(&lv_font_montserrat_14);
    gui().infoTiles.setFont(&lv_font_montserrat_12);

    initArc(gui().arcBg);
    initArc(gui().arcMeasured);
    initArc(gui().arcMeasuredEnd);
    initArc(gui().arcMeasuredBorder);
    initArc(gui().arcTarget);
    initArc(gui().arcTargetEnd);
    initArc(gui().arcTargetBorder);
}

void SimpleScreen::initArc(LvArc& arc){
    arc.removeKnob();
    arc.setClickable(false);
    arc.setArcRotation(90);
}

void SimpleScreen::onStart(){
    // to be implemented
}

void SimpleScreen::onBecomeEnabled(){
    // to be implemented
}

void SimpleScreen::onBecomeDisabled(){
    // to be implemented
}

void SimpleScreen::onUserInputTick(){
    // to be implemented
}

void SimpleScreen::onRender(){

    const MathFunctionIfc& fn = MathFunctions::GetSinAnim();

    const float speed = 0.2f;
    const float t = fmodf(Time::GetTickMicros() * 1e-6f * speed, 1.0f);
    const uint16_t angle = static_cast<uint16_t>(fn.evaluate(t) * 300.0f + 30.0f);

    LvArc& arc = gui().arcTarget;
    arc.setArcEndAngle(angle);

    LvArc& end = gui().arcTargetEnd;
    end.setArcAngles(angle, angle+1);

    LvArc& border = gui().arcTargetBorder;
    border.setArcEndAngle(angle);
}

} // namespace Garbox