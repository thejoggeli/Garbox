#pragma once

#include <functional>

#include "core/assert/Assert.h"

namespace Garbox {

class EventHeader;

class EventForwarder {
public:

    using Handler = std::function<void(const EventHeader* event)>;

    EventForwarder();

    void setHandler(Handler handler);
    void forward(const EventHeader* event);

private:
    Handler mHandler = nullptr;
};

} // namespace Garbox
