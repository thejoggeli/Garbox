#include "LookupFunction.h"

#include <cstring>
#include <cmath>
#include <limits>
#include <new>
#include "core/assert/Assert.h"

namespace {

// checks if x-values are strictly increasing
bool isMonotonicIncreasing(const Garbox::LookupFunction::Point* points, uint32_t count){
    if(points == nullptr){
        return false;
    }
    for(uint32_t i = 0; i < count; ++i){
        if(!std::isfinite(points[i].x) || !std::isfinite(points[i].y)){
            return false;
        }
    }
    for(uint32_t i = 1; i < count; ++i){
        if(points[i].x <= points[i - 1].x){
            return false;
        }
    }
    return true;
}

} // namespace

namespace Garbox {

LookupFunction::LookupFunction(){
    // nothing to do
}

LookupFunction::~LookupFunction(){
    if(mHeapAllocated){
        delete[] mPoints;
    }
}

void LookupFunction::init(const Point* points, bool copy, uint32_t pointCount, Mode mode){
    AssertExit(!mInitialized, "LookupFunction", "already initialized");
    if(mInitialized){
        return;
    }
    AssertExit(points != nullptr, "LookupFunction", "null point buffer");
    AssertExit(pointCount >= 2, "LookupFunction", "at least two points required");
    AssertExit(isMonotonicIncreasing(points, pointCount), "LookupFunction", "x-values must be strictly increasing");
    if(points == nullptr || pointCount < 2 || !isMonotonicIncreasing(points, pointCount)){
        return;
    }

    mPointCount = pointCount;
    mLastIndex = mPointCount - 1;
    mHeapAllocated = copy;
    mMode = mode;

    if(copy){
        Point* ownedPoints = new (std::nothrow) Point[mPointCount];
        AssertExit(ownedPoints != nullptr, "LookupFunction", "point allocation failed");
        if(ownedPoints == nullptr){
            mHeapAllocated = false;
            return;
        }
        std::memcpy(ownedPoints, points, mPointCount * sizeof(Point));
        mPoints = ownedPoints;
    }
    else {
        mPoints = points;
    }

    mInitialized = true;
}

void LookupFunction::init(std::initializer_list<Point> points, Mode mode){
    AssertExit(!mInitialized, "LookupFunction", "already initialized");
    if(mInitialized){
        return;
    }
    const bool sizeFits = points.size() <= std::numeric_limits<uint32_t>::max();
    const uint32_t pointCount = sizeFits ? static_cast<uint32_t>(points.size()) : 0;
    AssertExit(sizeFits, "LookupFunction", "too many points");
    AssertExit(pointCount >= 2, "LookupFunction", "at least two points required");
    AssertExit(isMonotonicIncreasing(points.begin(), pointCount), "LookupFunction", "x-values must be finite and strictly increasing");
    if(!sizeFits || pointCount < 2 || !isMonotonicIncreasing(points.begin(), pointCount)){
        return;
    }

    mPointCount = pointCount;
    mLastIndex = mPointCount - 1;
    mHeapAllocated = true;
    mMode = mode;

    Point* ownedPoints = new (std::nothrow) Point[mPointCount];
    AssertExit(ownedPoints != nullptr, "LookupFunction", "point allocation failed");
    if(ownedPoints == nullptr){
        mHeapAllocated = false;
        return;
    }
    mPoints = ownedPoints;
    uint32_t index = 0;
    for(const Point& p : points){
        ownedPoints[index++] = p;
    }

    mInitialized = true;
}

float LookupFunction::evaluate(float x) const {
    if(!mInitialized){
        TriggerDebug("LookupFunction", "evaluate() called before init()");
        return 0.0f;
    }
    if(!std::isfinite(x)){
        AssertExit(false, "LookupFunction", "input must be finite");
        return mPoints[0].y;
    }

    // clamp to range edges
    if(x <= mPoints[0].x){
        return mPoints[0].y;
    }
    else if(x >= mPoints[mLastIndex].x){
        return mPoints[mLastIndex].y;
    }

    // binary search to find the surrounding interval
    uint32_t low = 0;
    uint32_t high = mLastIndex;

    while((high - low) > 1){
        const uint32_t mid = (low + high) / 2;
        if(x < mPoints[mid].x){
            high = mid;
        }
        else {
            low = mid;
        }
    }

    const Point& p0 = mPoints[low];
    const Point& p1 = mPoints[high];

    if(mMode == Mode::Hold){
        // zero-order hold: return left sample
        return p0.y;
    }

    // linear interpolation
    const float t = (x - p0.x) / (p1.x - p0.x);
    return p0.y + (p1.y - p0.y) * t;
}

} // namespace Garbox
