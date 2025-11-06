#pragma once

#include <cstdint>
#include <initializer_list>
#include "FunctionIfc.h"

namespace Garbox {

class LookupTableFunction : public FunctionIfc {
public:
    struct Point {
        float x;
        float y;
    };

    LookupTableFunction();
    ~LookupTableFunction();

    // Initialize from external point buffer
    void init(Point* points, bool copy, uint32_t pointCount);

    // Initialize from initializer list (copied into internal buffer)
    void init(std::initializer_list<Point> points);

    // Evaluate with linear interpolation between stored points
    float evaluate(float x) const final;

private:
    Point* mPoints = nullptr;
    uint32_t mPointCount = 0;
    uint32_t mLastIndex = 0;
    bool mHeapAllocated = false;
    bool mInitialized = false;
};

} // namespace Garbox
