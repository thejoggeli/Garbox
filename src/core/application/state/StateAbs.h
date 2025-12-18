#pragma once

#include <cstdint>
#include "core/application/state/StateHostIfc.h"
#include "shared/types/StateType.h"

namespace Garbox {

class RuntimeAbs;

class StateAbs {
public:

    StateAbs(StateType type);
    ~StateAbs() = default;

    bool isDirty() const;
    StateType type() const;

protected:

    void setDirty(); // to be called in inheriting class

private:

    StateHostIfc* mHost = nullptr;
    StateType mType;
    bool mDirty = false;

    void init(StateHostIfc* host); // called in RuntimeAbs
    virtual void publish() = 0; // called in RuntimeAbs
    void clearDirty(); // called in RuntimeAbs

    // disallow copy and move 
    StateAbs(const StateAbs&) = delete;
    StateAbs& operator=(const StateAbs&) = delete;
    StateAbs(StateAbs&&) = delete;
    StateAbs& operator=(StateAbs&&) = delete;

    friend class RuntimeAbs;

};

} // namespace