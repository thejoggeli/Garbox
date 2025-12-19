// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"
#include "shared/types/BehaviourId.h"

namespace Garbox {

class ActiveBehaviourState final : public StateAbs {
public:
    ActiveBehaviourState();

    // read access (published state)
    BehaviourId getBehaviour() const;

    // read access (next state)
    BehaviourId nextBehaviour() const;

    // read access (last state)
    BehaviourId lastBehaviour() const;

    // write access (next state)
    void setBehaviour(BehaviourId value);

private:

    struct Data {
        BehaviourId behaviour;
    };

    Data mCurrent {};
    Data mNext {};
    Data mLast {};

    void publish() final;
};

} // namespace Garbox