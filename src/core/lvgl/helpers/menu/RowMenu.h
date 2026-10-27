#pragma once

#include <cstdint>

#include "core/util/container/heap/VectorHeap.h"
#include "core/lvgl/objects/LvObject.h"
#include "RowMenuRow.h"

namespace Garbox {

class RowMenu {
public:

    using ValueToTextCallback = void (*)(
        void* callbackCtx,
        uint8_t rowIndex,
        int32_t value,
        char* outText,
        uint32_t maxLen
    );

    using ChangeValueCallback = int32_t (*)(
        void* callbackCtx,
        uint8_t rowIndex,
        int32_t currentValue,
        int32_t delta
    );

    RowMenu(
        LvObject& container,
        uint32_t normalTextColor,
        uint32_t activeTextColor,
        uint32_t normalBgColor,
        uint32_t activeBgColor,
        uint32_t editTextColor,
        uint32_t editBgColor,
        uint8_t totalRowCount,
        uint8_t visibleRowCount,
        void* callbackCtx,
        ValueToTextCallback valueToTextCallback,
        ChangeValueCallback changeValueCallback
    );

    void addRow(const char* name, int32_t initialValue);

    void setSelectedRow(uint8_t index);
    void selectNextRow();
    void selectPreviousRow();

    void toggleLock();

    void nextValue();
    void previousValue();
    void changeSelectedValue(int32_t delta);

    void clearSelection();

    bool hasSelection() const;
    bool isLocked() const;
    uint8_t getSelectedRow() const;
    uint8_t getWindowStartRow() const;

    int32_t getValue(uint8_t rowIndex) const;
    void setValue(uint8_t rowIndex, int32_t value);

    void render();

    void setPad(int32_t left, int32_t right, int32_t top, int32_t bottom);

private:

    void renderWindow();
    void renderActive();
    static void fallbackValueToText(
        void*, uint8_t, int32_t, char* outText, uint32_t maxLen
    );
    static int32_t fallbackChangeValue(
        void*, uint8_t, int32_t currentValue, int32_t
    );

private:

    LvObject& mContainer;
    VectorHeap<RowMenuRow> mRows;

    const lv_color_t mNormalTextColor;
    const lv_color_t mActiveTextColor;

    const lv_color_t mNormalBgColor;
    const lv_color_t mActiveBgColor;
    const lv_color_t mEditTextColor;
    const lv_color_t mEditBgColor;

    uint8_t mVisibleRowCount;
    uint8_t mActiveRowIndex = 0;
    uint8_t mWindowStartIndex = 0;

    bool mHasSelection = false;
    bool mLocked = false;
    bool mNeedsRenderWindow = true;
    bool mNeedsRenderActive = true;

    void* mCallbackCtx = nullptr;
    ValueToTextCallback mValueToTextCallback;
    ChangeValueCallback mChangeValueCallback;
    
    friend class RowMenuRow;
};

} // namespace Garbox
