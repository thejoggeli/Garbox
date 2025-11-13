#include "ColorMap.h"

#include <algorithm>
#include <cmath>
#include "assert/Assert.h"
#include "util/color/ColorInterpolator.h"
#include "util/function/default/GammaFunctions.h"

namespace Garbox {

// Uniform RGB
ColorMap::ColorMap(std::initializer_list<RgbFloat> stops){

    // prepare samples
    initializeUniform(stops.size());

    // create color sample entry for each stop
    for(const RgbFloat& srgb : stops){
        const HslColor hsl = HslColor::From(srgb);
        const RgbFloat linearRgb = srgb.toLinearRgb();
        const LabColor lab = LabColor::From(linearRgb);
        mSamples.emplace_back(srgb, linearRgb, hsl, lab);
    }
}

// Uniform HSL
ColorMap::ColorMap(std::initializer_list<HslColor> stops){

    // prepare samples
    initializeUniform(stops.size());

    // create color sample entry for each stop
    for(const HslColor& hsl : stops){
        const RgbFloat sRgb = RgbFloat::From(hsl);
        const RgbFloat linearRgb = sRgb.toLinearRgb();
        const LabColor lab = LabColor::From(linearRgb);
        mSamples.emplace_back(sRgb, linearRgb, hsl, lab);
    }
}

// Non-uniform RGB
ColorMap::ColorMap(std::initializer_list<RgbStop> stops){

    // constructor body
    initializeFromRgbStops(stops);
}

// Non-uniform HSL
ColorMap::ColorMap(std::initializer_list<HslStop> stops){

    // constructor body
    initializeFromHslStops(stops);
}

void ColorMap::initializeFromRgbStops(std::initializer_list<RgbStop> stops){

    initializeFromStops(stops);
    const size_t expectedSize = mSamples.capacity();

    // initialize first stop
    auto it = stops.begin();
    float tPrev = it->t;
    RgbFloat srgbPrev = it->color;
    RgbFloat linPrev = srgbPrev.toLinearRgb();
    HslColor hslPrev = HslColor::From(srgbPrev);
    LabColor labPrev = LabColor::From(linPrev);

    while(++it != stops.end()){
        const float tCurr = it->t;

        // get current stop color
        const RgbFloat srgbCurr = it->color;
        const RgbFloat linCurr = srgbCurr.toLinearRgb();
        const HslColor hslCurr = HslColor::From(srgbCurr);
        const LabColor labCurr = LabColor::From(linCurr);

        // last sample, no further interpolation
        if(it == stops.end()){
            mSamples.emplace_back(srgbCurr, linCurr, hslCurr, labCurr);
            break;
        }

        // add interpolated colors (interpolate from current stop to next step)
        const size_t segSamples = static_cast<size_t>(std::round((tCurr - tPrev) / mSampleStep));
        for(size_t s = 0; s < segSamples; ++s){
            const float t = tPrev + static_cast<float>(s) * mSampleStep;
            const float frac = (t - tPrev) / mSampleStep;

            // interpolate colors
            const RgbFloat sRgb = ColorInterpolator::interpolateRgb(srgbPrev, srgbCurr, frac);
            const RgbFloat linearRgb = ColorInterpolator::interpolateRgb(linPrev, linCurr, frac);
            const HslColor hsl = ColorInterpolator::interpolateHsl(hslPrev, hslCurr, frac);
            const LabColor lab = ColorInterpolator::interpolateLab(labPrev, labCurr, frac);

            // add interpolated colors to samples vector
            mSamples.emplace_back(sRgb, linearRgb, hsl, lab);
        }

        // shift
        tPrev = tCurr;
        srgbPrev = srgbCurr;
        linPrev = linCurr;
        hslPrev = hslCurr;
        labPrev = labCurr;
    }

    // add last stop
    mSamples.emplace_back(srgbPrev, linPrev, hslPrev, labPrev);

    // ensure vector size did not change
    AssertExit(expectedSize == mSamples.size(), "ColorMap", "samples size must remain unchanged");
}

void ColorMap::initializeFromHslStops(std::initializer_list<HslStop> stops){

    initializeFromStops(stops);
    const size_t expectedSize = mSamples.capacity();

    // initialize first stop
    auto it = stops.begin();
    float tPrev = it->t;
    HslColor hslPrev = it->color;
    RgbFloat srgbPrev = RgbFloat::From(hslPrev);
    RgbFloat linPrev = srgbPrev.toLinearRgb();
    LabColor labPrev = LabColor::From(linPrev);

    while(++it != stops.end()){
        const float tCurr = it->t;

        // add current stop color
        const HslColor hslCurr = it->color;
        const RgbFloat srgbCurr = RgbFloat::From(hslCurr);
        const RgbFloat linCurr = srgbCurr.toLinearRgb();
        const LabColor labCurr = LabColor::From(linCurr);
        
        // last sample, no further interpolation
        if(it == stops.end()){
            break;
        }

        // add interpolated colors (interpolate from current stop to next step)
        const size_t segSamples = static_cast<size_t>(std::round((tCurr - tPrev) / mSampleStep));
        for(size_t s = 0; s < segSamples; ++s){
            const float t = tPrev + static_cast<float>(s) * mSampleStep;
            const float frac = (t - tPrev) / mSampleStep;

            // interpolate colors
            const HslColor hsl       = ColorInterpolator::interpolateHsl(hslPrev, hslCurr, frac);
            const RgbFloat sRgb      = ColorInterpolator::interpolateRgb(srgbPrev, srgbCurr, frac);
            const RgbFloat linearRgb = ColorInterpolator::interpolateRgb(linPrev, linCurr, frac);
            const LabColor lab       = ColorInterpolator::interpolateLab(labPrev, labCurr, frac);

            // add interpolated colors to samples vector
            mSamples.emplace_back(sRgb, linearRgb, hsl, lab);
        }

        // shift
        tPrev = tCurr;
        hslPrev = hslCurr;
        srgbPrev = srgbCurr;
        linPrev = linCurr;
        labPrev = labCurr;
    }

    // add last stop
    mSamples.emplace_back(srgbPrev, linPrev, hslPrev, labPrev);

    // ensure vector size did not change
    AssertExit(expectedSize == mSamples.size(), "ColorMap", "samples size must remain unchanged");
}

ColorMap::~ColorMap(){
    TriggerExit("ColorMap", "heap using classes must not be deconstructed");
}

const ColorMap::ColorSample& ColorMap::getSample(size_t index) const {
    const size_t clamped = std::min(index, (mSamples.size() > 0 ? mLastIndex : 0));
    return mSamples[clamped];
}

size_t ColorMap::size() const {
    return mSamples.size();
}

float ColorMap::step() const {
    return mSampleStep;
}

RgbFloat ColorMap::interpolateStandardRgb(float t) const {
    size_t i0, i1;
    float frac;
    indexAndFrac(t, i0, i1, frac);
    return ColorInterpolator::interpolateRgb(mSamples[i0].sRgb, mSamples[i1].sRgb, frac);
}

RgbFloat ColorMap::interpolateLinearRgb(float t) const {
    size_t i0, i1;
    float frac;
    indexAndFrac(t, i0, i1, frac);
    return ColorInterpolator::interpolateRgb(mSamples[i0].linear, mSamples[i1].linear, frac);
}

HslColor ColorMap::interpolateHsl(float t) const {
    size_t i0, i1;
    float frac;
    indexAndFrac(t, i0, i1, frac);
    return ColorInterpolator::interpolateHsl(mSamples[i0].hsl, mSamples[i1].hsl, frac);
}

LabColor ColorMap::interpolateLab(float t) const {
    size_t i0, i1;
    float frac;
    indexAndFrac(t, i0, i1, frac);
    return ColorInterpolator::interpolateLab(mSamples[i0].lab, mSamples[i1].lab, frac);
}

// ---------------- Helpers ----------------

void ColorMap::initializeUniform(size_t count){

    AssertExit((count >= 2), "ColorMap", "at least two colors required");
    
    // init members
    m_tMin = 0.0f;
    m_tMax = 1.0f;
    mSampleStep = 1.0f / static_cast<float>(count - 1);

    // init samples vector
    prepareSamples(count);
}

template<typename StopType>
void ColorMap::initializeFromStops(std::initializer_list<StopType> stops){

    AssertExit((stops.size() >= 2), "ColorMap", "at least two stops required");

    // compute sample step
    {
        auto it = stops.begin();
        float prev = it->t;
        ++it;

        // check if t are monotonic 
        for(; it != stops.end(); ++it){
            const float cur = it->t;
            AssertExit((prev <= cur), "ColorMap", "t must be strictly monotonic");
            prev = cur;
        }

        // find min step size => min(t[n] - t[n-1])
        float minStep = 1.0f;
        for(; it != stops.end(); ++it){
            const float cur = it->t;
            const float step = (cur - prev);
            if(step < minStep){
                minStep = step;
            }
            prev = cur;
        }

        // set sample step to min step
        mSampleStep = minStep;
        AssertExit((mSampleStep >= MinSamplingStep), "ColorMap", "sampling step too small");
    }

    // compute tMin and tMax
    {
        m_tMin = stops.begin()->t;
        m_tMax = std::prev(stops.end())->t;
    }

    // compute number of samples
    const size_t numSamples = static_cast<size_t>(std::round((m_tMax - m_tMin) / mSampleStep) + 1.0f);

    // init samples vector
    prepareSamples(numSamples);
}

void ColorMap::prepareSamples(size_t count){
    AssertExit(mSamples.capacity() == 0, "ColorMap", "expected unused vector");
    mSamples.reserve(count);
    mLastIndex = count - 1;
}

// Map t to neighboring pre-samples and fractional position
void ColorMap::indexAndFrac(float t, size_t& i0, size_t& i1, float& frac) const {

    // clamp t to [0,1]
    const float tClamped = std::clamp(t, 0.0f, 1.0f);

    // example:
    // tClamped = 0.85 and mSampleStep = 0.2
    // => 0.85 / 0.2 = 4.25 
    // => idx = 4
    // => frac = 0.25 
    const float pos = tClamped / mSampleStep; 
    const size_t idx = static_cast<size_t>(pos);

    // arrived at end samples vector
    // works if t is not in range [0, 1]
    if(idx >= mLastIndex){
        i0 = mLastIndex - 1;
        i1 = mLastIndex;
        frac = 1.0f;
        return;
    }

    i0 = idx;
    i1 = idx + 1;
    frac = pos - static_cast<float>(idx);
}

} // namespace Garbox
