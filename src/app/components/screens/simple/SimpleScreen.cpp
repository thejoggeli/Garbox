#include "SimpleScreen.h"
// This file was initially generated and is no longer modified by the generator.
// Interface changes must be applied manually in this file.
// See the corresponding header for the interface method declarations.

namespace Garbox {

SimpleScreen::SimpleScreen() : SimpleScreenAbs(){
    // to be implemented
}

void SimpleScreen::onInit(){
    gui().tempLabel.setFont(&lv_font_montserrat_12);
    gui().tempValue.setFont(&lv_font_montserrat_30);
    gui().tempUnit.setFont(&lv_font_montserrat_14);
    gui().infoTiles.setFont(&lv_font_montserrat_12);

    gui().arcBg.removeKnob();
    gui().arcBg.setClickable(false);

    gui().arcMeasured.removeKnob();
    gui().arcMeasured.setClickable(false);

    gui().arcTarget.removeKnob();
    gui().arcTarget.setClickable(false);

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
    // to be implemented
}

} // namespace Garbox