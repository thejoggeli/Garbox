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

    enum class Mode : uint8_t {
        Linear, // standard linear interpolation
        Hold    // zero-order hold (step)
    };

    LookupTableFunction();
    ~LookupTableFunction();

    void init(Point* points, bool copy, uint32_t pointCount, Mode mode = Mode::Linear);
    void init(std::initializer_list<Point> points, Mode mode = Mode::Linear);

    float evaluate(float x) const;

private:
    Point* mPoints = nullptr;
    uint32_t mPointCount = 0;
    uint32_t mLastIndex = 0;
    bool mHeapAllocated = false;
    bool mInitialized = false;
    Mode mMode = Mode::Linear;
};

} // namespace Garbox
