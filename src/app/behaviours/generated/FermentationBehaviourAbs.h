// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "app/behaviours/generated/BaseBehaviourAbs.h"

namespace Garbox {

class FermentationBehaviourAbs : public BaseBehaviourAbs {
public:

    FermentationBehaviourAbs(const RuntimeContext& context);

protected:

    // shorthand types for write events 
    using FanCommandWrite = EventWrite<EventPayload::FanCommand>;
    using BacklightCommandWrite = EventWrite<EventPayload::BacklightCommand>;
    using HeatpadCommandWrite = EventWrite<EventPayload::HeatpadCommand>;

    // make typed events
    FanCommandWrite makeFanCommandEvent();
    BacklightCommandWrite makeBacklightCommandEvent();
    HeatpadCommandWrite makeHeatpadCommandEvent();

    // send typed events
    void sendEvent(const FanCommandWrite& event);
    void sendEvent(const BacklightCommandWrite& event);
    void sendEvent(const HeatpadCommandWrite& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEvent;

};

} // namespace Garbox