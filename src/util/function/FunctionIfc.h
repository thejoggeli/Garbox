// FunctionIfc.h
#pragma once

namespace Garbox {

class FunctionIfc {
public:
    virtual ~FunctionIfc() = default;
    virtual float evaluate(float x) const = 0;
};

} // namespace Garbox
