#include "SafeOutputManager.h"

#include "app/providers/PartsProvider.h"
#include "core/assert/Assert.h"
#include "core/safety/SafeOutputRegistry.h"
#include "modules/parts/heatpad/Heatpad.h"

namespace Garbox {
namespace {

SafeOutputRegistry<4U> sRegistry;
bool sInitialized = false;

void StopHeatpad(void* context){
    static_cast<Heatpad*>(context)->forceSafeOutput();
}

} // namespace

void SafeOutputManager::Init(){
    AssertExit(!sInitialized, "SafeOutputManager", "already initialized");
    AssertExit(sRegistry.registerHandler(StopHeatpad, &PartsProvider::GetHeatpad()),
               "SafeOutputManager", "heatpad registration failed");
    sInitialized = true;
}

void SafeOutputManager::Apply(){
    // Empty/uninitialized application is safe during early boot because the
    // generated hardware defaults leave the heater enable output low.
    sRegistry.apply();
}

bool SafeOutputManager::IsApplied(){
    return sRegistry.applied();
}

} // namespace Garbox
