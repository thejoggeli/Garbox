#include "ColorMap.h"

#include <algorithm>
#include <cmath>
#include "assert/Assert.h"
#include "util/color/ColorInterpolator.h"

namespace Garbox {

ColorMap::Entry::Entry(float tValue, const HslColor& hslColor):
    // initialize members (from hsl color)
    t(tValue),
    standardRgb(RgbFloat::From(hslColor)),
    linearRgb(standardRgb.toLinearRgb()),
    hsl(hslColor),
    lab(LabColor::From(linearRgb)){
    // nothing to do
}

ColorMap::Entry::Entry(float tValue, const RgbFloat& rgbColor):
    // initialize members (from standard rgb color)
    t(tValue),
    standardRgb(rgbColor),
    linearRgb(rgbColor.toLinearRgb()),
    hsl(HslColor::From(rgbColor)),
    lab(LabColor::From(linearRgb)){
    // nothing to do
}

ColorMap::Entry::Entry(const HslColor& hslColor):
    // initialize members (from standard rgb color)
    t(0.0f),
    standardRgb(RgbFloat::From(hslColor)),
    linearRgb(standardRgb.toLinearRgb()),
    hsl(hslColor),
    lab(LabColor::From(linearRgb)){
    // nothing to do
}

void ColorMap::LookupBucket::set(uint8_t startIndexVal, uint8_t endIndexVal){
    startIndex = startIndexVal;
    endIndex = endIndexVal;
    size = endIndex - startIndex;
}

ColorMap::Entry::Entry(const RgbFloat& rgbColor):
    // initialize members (from standard rgb color)
    t(0.0f),
    standardRgb(rgbColor),
    linearRgb(rgbColor.toLinearRgb()),
    hsl(HslColor::From(standardRgb)),
    lab(LabColor::From(linearRgb)){
    // nothing to do
}

ColorMap::ColorMap(const Span<const Entry> entries) : mEntries(entries){
    init();
}

ColorMap::~ColorMap(){
    TriggerExit("ColorMap", "heap using classes must not be deconstructed");
}

void ColorMap::init(){
    AssertExit(!mInitialized, "ColorMap", "already initialized");

    const size_t numEntries = mEntries.size();
    AssertExit((numEntries >= 2u), "ColorMap", "need at least two entries");
    AssertExit((numEntries <= 256U), "ColorMap", "max 256 entries allowed");

    // check if t uniform
    mUniform = checkUniform();

    // handle uniform
    if(mUniform){
        mUniformStep = 1.0f / static_cast<float>(numEntries - 1);
    }
    // handle non-uniform
    else {

        AssertExit(mEntries[0].t == 0.0f, "ColorMap", "first t must to 0.0");
        AssertExit((mEntries.end()-1)->t == 1.0f, "ColorMap", "last t must be 1.0");

        // check if t is strictly monotonic 
        for(size_t i = 1u; i < numEntries; ++i){
            AssertExit((mEntries[i].t > mEntries[i - 1].t), "ColorMap", "t values must be monotonic");
        }

        // build lookup buckets 
        buildLookupBuckets();
    }

    mInitialized = true;
}

bool ColorMap::checkUniform() const {

    const size_t numEntries = mEntries.size();

    // check if all t values are zero
    bool allZero = true;
    for(size_t i = 0; i < numEntries; ++i){
        if(mEntries[i].t != 0.0f){
            allZero = false;
        }
    }

    // all zero => uniform distribution
    if(allZero){
        return true;
    }

    // check if all steps sizes are sime
    const float tolerance = 1e-4f;

    // check lower boundary
    if(std::fabs(mEntries.front().t - 0.0f) > tolerance){
        return false;
    }

    // check upper boundary
    if(std::fabs(mEntries.back().t - 1.0f) > tolerance){
        return false;
    }

    // check if all steps sizes are same
    const float step = mEntries[1].t - mEntries[0].t;
    for(size_t i = 1; i < numEntries; ++i){
        float delta = mEntries[i].t - mEntries[i - 1].t;
        if(std::fabs(delta - step) > tolerance){
            return false;
        }
    }

    return true;
}

void ColorMap::buildLookupBuckets(){
    const size_t numEntries = mEntries.size();
    const size_t lastSeg = numEntries - 2;
    const float bucketStep = 1.0f / static_cast<float>(BucketCount);

    size_t entryIndexLower = 0; // segment index for lower bound
    size_t entryIndexUpper = 0; // segment index for upper bound

    for(size_t b = 0; b < BucketCount; ++b){
        const float t0 = static_cast<float>(b) * bucketStep;
        const float t1 = t0 + bucketStep;

        // advance lower bound to just before the bucket boundary
        while(entryIndexLower < lastSeg && mEntries[entryIndexLower+1].t <= t0){
            entryIndexLower++;
        }

        // advance upper bound to just after the boucket boundary
        while(entryIndexUpper < lastSeg && mEntries[entryIndexUpper].t < t1){
            entryIndexUpper++;
        }

        size_t bucketStartIndex = entryIndexLower;
        size_t bucketEndIndex = entryIndexUpper + 1;

        // upper limit for start index
        if(bucketStartIndex > (numEntries-2)){
            bucketStartIndex = numEntries-2;
        }

        // upper limit for end index
        if(bucketEndIndex > (numEntries-1)){
            bucketEndIndex = numEntries-1;
        }

        mLookup[b].set(static_cast<uint8_t>(bucketStartIndex), static_cast<uint8_t>(bucketEndIndex));
    }
}

void ColorMap::resolveSegmentUniform(float tClamped, SegmentInfo* segmentInfo) const {
    float pos = tClamped / mUniformStep;
    size_t idx = static_cast<size_t>(pos);

    const size_t maxIndex = mEntries.size() - 2;
    if(idx > maxIndex){
        idx = maxIndex;
    }

    segmentInfo->a = &mEntries[idx];
    segmentInfo->b = &mEntries[idx+1];
    segmentInfo->frac = pos - static_cast<float>(idx);
}

void ColorMap::resolveSegmentNonUniform(float tClamped, SegmentInfo* segmentInfo) const {

    // find lookup bucket index
    size_t bucketIndex = static_cast<size_t>(tClamped * static_cast<float>(BucketCount));
    if(bucketIndex >= BucketCount){
        bucketIndex = BucketCount - 1u;
    }

    // get entry start and end bounding indices from bucket
    const LookupBucket& bucket = mLookup[bucketIndex];
    size_t startIndex = bucket.startIndex;
    size_t endIndex = bucket.endIndex;

    // find entry index
    size_t entryIndex = endIndex;
    if(bucket.size > 1){
        // loop until entry pair found were tLower <= t <= tUpper
        // if no entry found, entryIndex will be last entry in bucket
        for(size_t i = startIndex; i < endIndex; ++i){
            if(tClamped >= mEntries[i].t && tClamped <= mEntries[i + 1u].t){
                entryIndex = i;
                break;
            }
        }
    }

    // constraint upper index
    const size_t maxIndex = mEntries.size() - 2;
    if(entryIndex > maxIndex){
        entryIndex = maxIndex;
    }

    // create segment
    const Entry& entryA = mEntries[entryIndex];
    const Entry& entryB = mEntries[entryIndex+1];
    segmentInfo->a = &entryA;
    segmentInfo->b = &entryB;
    segmentInfo->frac = (tClamped - entryA.t) / (entryB.t - entryA.t);
}


void ColorMap::resolveSegment(float t, SegmentInfo* segmentInfo) const {
    float tClamped = std::clamp(t, 0.0f, 1.0f);

    if(mUniform){
        resolveSegmentUniform(tClamped, segmentInfo);
    }
    else {
        resolveSegmentNonUniform(tClamped, segmentInfo);
    }
}

const ColorMap::Entry& ColorMap::getEntry(size_t index) const {
    const size_t lastIndex = mEntries.size() - 1u;
    const size_t clamped = (index < mEntries.size() ? index : lastIndex);
    return mEntries[clamped];
}

size_t ColorMap::size() const {
    return mEntries.size();
}

RgbFloat ColorMap::interpolateStandardRgb(float t) const {
    SegmentInfo seg;
    resolveSegment(t, &seg);
    return ColorInterpolator::interpolateRgb(
        seg.a->standardRgb, 
        seg.b->standardRgb, 
        seg.frac
    );
}

RgbFloat ColorMap::interpolateLinearRgb(float t) const {
    SegmentInfo seg;
    resolveSegment(t, &seg);
    return ColorInterpolator::interpolateRgb(
        seg.a->linearRgb, 
        seg.b->linearRgb, 
        seg.frac
    );
}

HslColor ColorMap::interpolateHsl(float t) const {
    SegmentInfo seg;
    resolveSegment(t, &seg);
    return ColorInterpolator::interpolateHsl(
        seg.a->hsl, 
        seg.b->hsl, 
        seg.frac
    );
}

LabColor ColorMap::interpolateLab(float t) const {
    SegmentInfo seg;
    resolveSegment(t, &seg);
    return ColorInterpolator::interpolateLab(
        seg.a->lab,
        seg.b->lab,
        seg.frac
    );
}

} // namespace Garbox
