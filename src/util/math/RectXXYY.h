#pragma once

#include <cstdint>
#include <algorithm>

namespace Garbox {

template<typename T>
class RectXXYY {
public:
    T x1;
    T x2;
    T y1;
    T y2;

    RectXXYY() : x1(0), x2(0), y1(0), y2(0) {
        // constructor body
    }

    RectXXYY(T x1Val, T x2Val, T y1Val, T y2Val): 
        x1(x1Val), 
        x2(x2Val), 
        y1(y1Val), 
        y2(y2Val){
        // constructor body
    }

    T width() const {
        return (x2 >= x1) ? (x2 - x1 + 1) : 0;
    }

    T height() const {
        return (y2 >= y1) ? (y2 - y1 + 1) : 0;
    }

    bool isValid() const {
        return (x2 >= x1) && (y2 >= y1);
    }

    void normalize() {
        if (x2 < x1) std::swap(x1, x2);
        if (y2 < y1) std::swap(y1, y2);
    }

    bool contains(T x, T y) const {
        return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
    }
};

} // namespace Garbox
