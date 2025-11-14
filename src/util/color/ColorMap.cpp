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

        // check if t is strictly monotonic 
        for(size_t i = 1u; i < numEntries; ++i){
            AssertExit((mEntries[i].t >= mEntries[i - 1].t), "ColorMap", "t values must be monotonic");
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

    size_t entryIndexA = 0; // segment index for start
    size_t entryIndexB = 0; // segment index for end

    for(size_t b = 0; b < BucketCount; ++b){
        const float t0 = static_cast<float>(b) * BucketStep;
        const float t1 = t0 + BucketStep;

        // advance A while the upper bound is still <= t0
        while(entryIndexA < lastSeg && mEntries[entryIndexA + 1].t <= t0){
            entryIndexA++;
        }

        // advance B while the lower bound is < t1
        while(entryIndexB < lastSeg && mEntries[entryIndexB].t < t1){
            entryIndexB++;
        }

        size_t bucketStartIndex = entryIndexA;
        size_t bucketEndIndex = entryIndexB + 1; // one-past-last segment

        // upper limit for start index
        if(bucketStartIndex > (numEntries-2)){
            bucketStartIndex = numEntries-2;
        }

        // upper limit for end index
        if(bucketEndIndex > (numEntries-1)){
            bucketEndIndex = numEntries-1;
        }

        mLookup[b].startIndex = static_cast<uint8_t>(bucketStartIndex);
        mLookup[b].endIndex = static_cast<uint8_t>(bucketEndIndex);
    }
}

void ColorMap::findEntryIndicesUniform(float t, size_t* index1, size_t* index2, float* frac) const {
    float pos = t / mUniformStep;
    size_t idx = static_cast<size_t>(pos);

    const size_t maxIndex = mEntries.size() - 2;
    if(idx > maxIndex){
        idx = maxIndex;
    }

    *index1 = idx;
    *index2 = idx+1;
    *frac = pos - static_cast<float>(idx);
}

void ColorMap::findEntryIndices(float t, size_t* index1, size_t* index2) const {
    float tClamped = std::clamp(t, 0.0f, 1.0f);

    size_t bucket = static_cast<size_t>(tClamped * static_cast<float>(BucketCount));
    if(bucket >= BucketCount){
        bucket = BucketCount - 1u;
    }

    const LookupBucket& b = mLookup[bucket];
    size_t startIndex = b.startIndex;
    size_t endIndex = b.endIndex;

    size_t idx = startIndex;

    if(endIndex > startIndex){
        bool found = false;
        for(size_t i = startIndex; i < endIndex; ++i){
            if(tClamped >= mEntries[i].t && tClamped <= mEntries[i + 1u].t){
                idx = i;
                found = true;
                break;
            }
        }

        if(!found){
            if(endIndex > 0u){
                idx = endIndex - 1u;
            }
            else {
                idx = 0u;
            }
        }
    }

    const size_t maxIndex = mEntries.size() - 2u;
    if(idx > maxIndex){
        idx = maxIndex;
    }

    *index1 = idx;
    *index2 = idx+1;
}


void ColorMap::resolveSegment(float t, SegmentInfo* segmentInfo) const {
    float tClamped = std::clamp(t, 0.0f, 1.0f);

    size_t idx1 = 0u;
    size_t idx2 = 0u;

    if(mUniform){
        findEntryIndicesUniform(tClamped, &idx1, &idx2, &segmentInfo->frac);
        segmentInfo->a = &mEntries[idx1];
        segmentInfo->b = &mEntries[idx2];
    }
    else {
        findEntryIndices(tClamped, &idx1, &idx2);
        const Entry& entryA = mEntries[idx1];
        const Entry& entryB = mEntries[idx2];
        segmentInfo->a = &entryA;
        segmentInfo->b = &entryB;
        if(idx1 == idx2){
            segmentInfo->frac = 1.0f;
        }
        else {
            segmentInfo->frac = (tClamped - entryA.t) / (entryB.t - entryA.t);
        }
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
