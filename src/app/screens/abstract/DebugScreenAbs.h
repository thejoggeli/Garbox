#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {
protected:

public:

    // receive event handlers (to be implmeneted by user)
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

    DebugScreenAbs();

    void init(ComponentHostIfc& host) final;

    class Model { 
    public: 

        // dirty-flag indices (one per group) 
        enum class Index : uint8_t { 
            Count 
        }; 

        Model(DebugScreenAbs& screen); 

        // disallow copy and move 
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;
        Model(Model&&) = delete;
        Model& operator=(Model&&) = delete;

        // getters

        // setters 

    private: 

        DebugScreenAbs& mScreen; 

        // all model fields

    };

    Model& getModel();
    const Model& getModel() const;

protected:

    Model mModel;

    // method to mark an model field index dirty (for manually updated values)
    void markDirty(Model::Index index);
    bool isMarkedDirty(Model::Index index) const;

    // make typed events 
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

    // hide dangerous raw access
    using ScreenAbs::registerUpdateHandler;
    using ScreenAbs::markDirtyRaw;
    using ScreenAbs::isMarkedDirtyRaw;

};

} // namespace Garbox