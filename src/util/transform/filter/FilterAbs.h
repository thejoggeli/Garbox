#pragma once

#include "FilterIfc.h"
#include "assert/Assert.h"

namespace Garbox {

/**
 * @brief FilterAbs
 *
 * Abstract base class for filters providing a consistent structure:
 * - Stores the last raw and filtered values
 * - Provides a final process() method that calls onProcess()
 * - Provides a final reset() that delegates to onReset()
 *
 * Derived classes only implement onProcess() and optionally onReset().
 */
class FilterAbs : public FilterIfc {
public:
    FilterAbs();
    virtual ~FilterAbs() = default;

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
