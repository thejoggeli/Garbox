#pragma once

#include "generated/screens/StateLogScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/static/RingBufferStatic.h"

namespace Garbox {

class StateLogScreen : public StateLogScreenAbs {
public:

    StateLogScreen();

    // any state changed handler (to be implmeneted by user)
    void onStateChanged(const StateAbs& state) final;

private:

    uint32_t mEntryCounter = 0;

    struct StateEntry {
        uint32_t id;
        StateType type;
    };

    struct LogRow {
        LvObject container;
        LvLabel marker;
        LvLabel id;
        LvLabel text;
        LogRow(LvObject& parent);
    };

    static constexpr size_t NumRows = 22;
    ArrayStatic<LogRow, NumRows> mRows;
    size_t mNextLabelIndex = 0;
    size_t mCurrentLabelIndex = static_cast<size_t>(-1);
    RingBufferStatic<StateEntry, NumRows> mBuffer;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; 

};

} // namespace Garbox