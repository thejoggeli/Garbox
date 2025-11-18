#pragma once

#include "FilterFunctionIfc.h"
#include "core/assert/Assert.h"

namespace Garbox {

/**
 * @brief FilterFunctionAbs
 *
 * Abstract base class for filters providing a consistent structure:
 * - Stores the last raw and filtered values
 * - Provides a final process() method that calls onProcess()
 * - Provides a final reset() that delegates to onReset()
 *
 * Derived classes only implement onProcess() and optionally onReset().
 */
class FilterFunctionAbs : public FilterFunctionIfc {
public:
    FilterFunctionAbs();
    virtual ~FilterFunctionAbs() = default;

    void reset(float value) final;
    float process(float value) final;
    float getFilteredValue() const final;
    float getRawValue() const final;

protected:
    virtual void onReset(float value);
    virtual float onProcess(float value) = 0;

    float mLastRaw = 0.0f;
    float mLastFiltered = 0.0f;
    bool mInitialized = false;
};

} // namespace Garbox
