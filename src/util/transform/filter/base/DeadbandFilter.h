#pragma once

#include "util/transform/filter/FilterAbs.h"
#include <cmath>

namespace Garbox {

/**
 * @brief DeadbandFilter
 *
 * Passes input through unchanged except when it changes by less than a threshold.
 * In that case, the output remains constant (no update).
 *
 * Typical use: eliminate jitter or sensor noise around a steady value.
 */
class DeadbandFilter : public FilterAbs {
public:
    DeadbandFilter();
    explicit DeadbandFilter(float threshold);

    void setThreshold(float threshold);
    float getThreshold() const;

protected:
    float onProcess(float value) override;

private:
    float mThreshold = 0.0f;
};

} // namespace Garbox
