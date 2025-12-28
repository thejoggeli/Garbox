#pragma once

#include <cstdint>

#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/lvgl/objects/LvObject.h"

namespace Garbox {

class RowMenu;

class RowMenuRow {
public:

    RowMenuRow(
        RowMenu& menu,
        LvObject& container,
        uint8_t rowIndex,
        const char* name,
        int32_t initialValue
    );

    void setVisible(bool visible);
    void setActive(bool rowActive, bool valueActive);

    void changeValue(int32_t delta);

    void setValue(int32_t value);
    int32_t getValue() const;

    void setPad(int32_t left, int32_t right, int32_t top, int32_t bottom);

    void render();

private:

    RowMenu& mMenu;
    const uint8_t mRowIndex;

    LvObject mContainer;
    LvLabel  mNameLabel;
    LvLabel  mValueLabel;

    int32_t mValue;

    bool mNeedsRender = true;

    static constexpr uint32_t ValueTextMaxLen = 16;
};

} // namespace Garbox
