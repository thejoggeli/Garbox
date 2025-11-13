#pragma once

#include <vector>
#include "util/color/types/HslColor.h"
#include "util/color/types/LabColor.h"
#include "util/color/types/RgbFloat.h"

namespace Garbox {

class ColorMap {
public:

    struct ColorSample {
        const RgbFloat sRgb;   // Standard RGB 
        const RgbFloat linear; // Linear RGB 
        const HslColor hsl;
        const LabColor lab;
        ColorSample(const RgbFloat& sRgbVal, const RgbFloat& linearVal, const HslColor& hslVal, const LabColor& labVal):
            // initialize members
            sRgb(sRgbVal), linear(linearVal), hsl(hslVal), lab(labVal){
            // constructor body
        }
    };

    // color must be in sRGB color space
    struct RgbStop {
        const float t;
        const RgbFloat color;
    };

    // color must be in HSL color space
    struct HslStop {
        const float t;
        const HslColor color;
    };

    // Uniform RGB stops (implicitly t = 0..1)
    // Assumes RGB colors in standard RGB (sRGB) color space
    ColorMap(std::initializer_list<RgbFloat> stops);

    // Uniform HSL stops (implicitly t = 0..1)
    ColorMap(std::initializer_list<HslColor> stops);

    // Non-uniform RGB stops (explicit t)
    // Assumes RGB colors in standard RGB (sRGB) color space
    ColorMap(std::initializer_list<RgbStop> stops); // BROKEN! DO NOT USE!

    // Non-uniform HSL stops (explicit t)
    ColorMap(std::initializer_list<HslStop> stops); // BROKEN! DO NOT USE!

    ~ColorMap();

    ColorMap(const ColorMap&) = delete;
    ColorMap& operator=(const ColorMap&) = delete;
    ColorMap(ColorMap&&) = delete;
    ColorMap& operator=(ColorMap&&) = delete;

    // Pre-sampled access
    const ColorSample& getSample(size_t index) const;
    size_t size() const; // returns number of samples
    float step() const; // returns step between samples

    // Runtime interpolation between pre-samples
    RgbFloat interpolateStandardRgb(float t) const; // returns color in standard RGB (sRGB) space
    RgbFloat interpolateLinearRgb(float t) const;   // returns color in linear RGB space
    HslColor interpolateHsl(float t) const;         // returns color in HSL space
    LabColor interpolateLab(float t) const;         // returns color in Lab space

private:

    static constexpr float MinSamplingStep = 0.05f; // minimum allowed sample step

    std::vector<ColorSample> mSamples;
    float mSampleStep = 0.0f;
    size_t mLastIndex = 0;
    float m_tMin = 0.0f;
    float m_tMax = 1.0f;

    // Setup
    template<typename StopType>
    void initializeFromStops(std::initializer_list<StopType> stops);
    void initializeUniform(size_t size);
    void prepareSamples(size_t count);

    // Builders (operate directly on the initializer_list, no copies)
    void initializeFromRgbStops(std::initializer_list<RgbStop> stops);
    void initializeFromHslStops(std::initializer_list<HslStop> stops);

    // Interp helper on pre-samples
    void indexAndFrac(float t, size_t& i0, size_t& i1, float& frac) const;
};

} // namespace Garbox
