#pragma once 

namespace Garbox {

class BehaviourIfc {
public:
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void setActive(bool active);
};

} // namespace