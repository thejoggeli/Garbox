// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DevtoolsControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DevtoolsControllerAbs::DevtoolsControllerAbs() : ControllerAbs(ComponentId::DevtoolsController, ControllerId::Devtools){
    // nothing to do
}

void DevtoolsControllerAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Logging: onLoggingTick(); break;
        default: TriggerDebug("DevtoolsControllerAbs", "received unhandled tick");
    };
};

void DevtoolsControllerAbs::receiveEvent(const EventHeader* header){
    // no events configured
};
 
} // namespace Garbox