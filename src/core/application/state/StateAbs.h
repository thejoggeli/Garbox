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

    void markDirty(); // to be called in publish()
    bool isDirty() const;
    StateType type() const;

protected:

    virtual void publish() = 0; // implemented in inheriting class
    void clearDirty(); // to be called in publish()

private:

    StateHostIfc* mHost = nullptr;
    StateType mType;
    bool mDirty = false;

    void init(StateHostIfc& host); // inject host in RuntimeAbs

    // disallow copy and move 
    StateAbs(const StateAbs&) = delete;
    StateAbs& operator=(const StateAbs&) = delete;
    StateAbs(StateAbs&&) = delete;
    StateAbs& operator=(StateAbs&&) = delete;

    friend class RuntimeAbs;

};

} // namespace