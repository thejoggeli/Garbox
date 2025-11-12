#pragma once

#include "util/function/FunctionIfc.h"

namespace Garbox {

/**
 * @brief FilterFunctionIfc
 *
 * Interface for stateful filters (e.g., smoothing, moving average, exponential).
 * 
 * Extends FunctionIfc to unify filters with other transformations.
 * Each filter must maintain internal state and implement reset/process semantics.
 */
class FilterFunctionIfc : public FunctionIfc {
public:
    virtual ~FilterFunctionIfc() = default;

    /**
     * @brief Reset the filter state to a specific value.
     * @param value Initial value for both raw and filtered states.
     */
    virtual void reset(float value) = 0;

    /**
     * @brief Process a new input value and update internal state.
     * @param value Input value.
     * @return Filtered output value.
     */
    virtual float process(float value) = 0;

    /**
     * @brief Get the most recently computed filtered output.
     */
    virtual float getFilteredValue() const = 0;

    /**
     * @brief Get the most recently received raw input value.
     */
    virtual float getRawValue() const = 0;

    /**
     * @brief Apply the transformation (FunctionIfc interface).
     * Delegates to process().
     */
    float apply(float value) final {
        return process(value);
    }
};

} // namespace Garbox
