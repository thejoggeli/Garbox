#pragma once 

namespace Garbox {

class StateAbs;

class StateHostIfc {
public:

    virtual void markStateDirty(StateAbs* header) = 0;

};

} // namespace
