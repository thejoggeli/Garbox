#include "LookupFunction.h"

#include <cstring>
#include "assert/Assert.h"

namespace {

// checks if x-values are strictly increasing
bool isMonotonicIncreasing(const Garbox::LookupFunction::Point* points, uint32_t count){
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
        // runtime destruction of heap-based lookup tables is not allowed
        TriggerExit("LookupFunction", "heap using classes must not be deconstructed");
    }
}

void LookupFunction::init(Point* points, bool copy, uint32_t pointCount, Mode mode){
    AssertExit(!mInitialized, "LookupFunction", "already initialized");
    AssertExit(points != nullptr, "LookupFunction", "null point buffer");
    AssertExit(pointCount >= 2, "LookupFunction", "at least two points required");
    AssertExit(isMonotonicIncreasing(points, pointCount), "LookupFunction", "x-values must be strictly increasing");

    mPointCount = pointCount;
    mLastIndex = mPointCount - 1;
    mHeapAllocated = copy;
    mMode = mode;

    if(copy){
        mPoints = new Point[mPointCount];
        std::memcpy(mPoints, points, mPointCount * sizeof(Point));
    }
    else {
        mPoints = points;
    }

    mInitialized = true;
}

void LookupFunction::init(std::initializer_list<Point> points, Mode mode){
    AssertExit(!mInitialized, "LookupFunction", "already initialized");
    AssertExit(points.size() >= 2, "LookupFunction", "at least two points required");
    AssertExit(isMonotonicIncreasing(points.begin(), points.size()), "LookupFunction", "x-values must be strictly increasing");

    mPointCount = static_cast<uint32_t>(points.size());
    mLastIndex = mPointCount - 1;
    mHeapAllocated = true;
    mMode = mode;

    mPoints = new Point[mPointCount];
    uint32_t index = 0;
    for(const Point& p : points){
        mPoints[index++] = p;
    }

    mInitialized = true;
}

float LookupFunction::evaluate(float x) const {
    if(!mInitialized){
        TriggerDebug("LookupFunction", "evaluate() called before init()");
        return 0.0f;
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
