#include "DisplayController.h"

#include "app/providers/PartsProvider.h"
#include "parts/display/Display.h"

namespace Garbox {

DisplayController::DisplayController(): 
    // init members
    ControllerAbs(ControllerId::Display),
    mDisplay(PartsProvider::GetDisplay()){
    // nothing to do
}

void DisplayController::onInit(){
    // display already initialized in parts provider
}

void DisplayController::onStart(){
    // nothing to do
}

void DisplayController::onTick(){
    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){
        mDisplay.giveRenderTrigger();
    }
}

} // namespace