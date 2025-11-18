#pragma once 

#include "core/application/behaviour/BehaviourIfc.h"

namespace Garbox {

class BehaviourAbs : public BehaviourIfc {
public:
    virtual void init();
    virtual void start();
    virtual void setActive(bool active);

protected:



};

} // namespace