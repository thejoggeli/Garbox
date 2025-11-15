#include "StaticLedGroup.h"

#include "assert/Assert.h"

namespace Garbox {

StaticLedGroup::StaticLedGroup(Span<StaticLed> leds) : mLeds(leds){
    // nothing to do
}

StaticLedGroup::~StaticLedGroup(){
    // nothing to do
}

void StaticLedGroup::init(){
    AssertExit(!mInitialized, "StaticLedGroup", "already initialized");

    // init leds
    for(StaticLed& led : mLeds){
        led.init();
    }

    mInitialized = true;
}

StaticLed& StaticLedGroup::getLed(uint8_t index){
    if(index >= mLeds.size()){
        TriggerExit("StaticLedGroup", "invalid static led index");
    }
    return mLeds[index];
}

Span<StaticLed>& StaticLedGroup::getAllLeds(){
    return mLeds;
}

void StaticLedGroup::setLed(uint8_t index, bool enabled){
    if(!mInitialized){
        TriggerDebug("StaticLedGroup", "not initialized");
        return;
    }

    if(index >= mLeds.size()){
        TriggerDebug("StaticLedGroup", "invalid static led index");
        return;
    }

    mLeds[index].setEnabled(enabled);
}

void StaticLedGroup::setAllLeds(bool enabled){
    if(!mInitialized){
        TriggerDebug("StaticLedGroup", "not initialized");
        return;
    }

    for(StaticLed& led : mLeds){
        led.setEnabled(enabled);
    }
}

void StaticLedGroup::toggleLed(uint8_t index){
    if(!mInitialized){
        TriggerDebug("StaticLedGroup", "not initialized");
        return;
    }

    if(index >= mLeds.size()){
        TriggerDebug("StaticLedGroup", "invalid static led index");
        return;
    }

    const bool current = mLeds[index].isEnabled();
    mLeds[index].setEnabled(!current);
}

void StaticLedGroup::toggleAllLeds(){
    if(!mInitialized){
        TriggerDebug("StaticLedGroup", "not initialized");
        return;
    }

    for(StaticLed& led : mLeds){
        const bool current = led.isEnabled();
        led.setEnabled(!current);
    }
}

bool StaticLedGroup::isInitialized() const {
    return mInitialized;
}

uint16_t StaticLedGroup::getCount() const {
    return mLeds.size();
}

} // namespace Garbox
