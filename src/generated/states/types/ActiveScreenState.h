// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class ActiveScreenState final : public StateAbs {
public:
    ActiveScreenState();

    // read access (published state)
    ScreenId getScreen() const;

    // read access (next state)
    ScreenId nextScreen() const;

    // write access (next state)
    void setScreen(ScreenId value);

private:

    struct Data {
        ScreenId screen;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox