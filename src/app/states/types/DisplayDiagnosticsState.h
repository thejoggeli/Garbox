// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class DisplayDiagnosticsState final : public StateAbs {
public:
    DisplayDiagnosticsState();

    // read access (published state)
    uint32_t getSkippedFrames() const;

    // read access (next state)
    uint32_t nextSkippedFrames() const;

    // write access (next state)
    void setSkippedFrames(uint32_t value);

private:

    struct Data {
        uint32_t skippedFrames;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox