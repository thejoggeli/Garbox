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

    // make typed events
    EventWrite<EventPayload::FanCommand> makeFanCommandEvent();
    EventWrite<EventPayload::BacklightCommand> makeBacklightCommandEvent();
    EventWrite<EventPayload::HeatpadCommand> makeHeatpadCommandEvent();

    // send typed events
    void sendEvent(const EventWrite<EventPayload::FanCommand>& event);
    void sendEvent(const EventWrite<EventPayload::BacklightCommand>& event);
    void sendEvent(const EventWrite<EventPayload::HeatpadCommand>& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEvent;

};

} // namespace Garbox