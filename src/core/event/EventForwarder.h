#pragma once

#include <functional>

#include "assert/Assert.h"
#include "core/event/Event.h"

namespace Garbox {

class EventForwarder {
public:

    using Handler = std::function<void(const Event* event)>;

    EventForwarder();

    void setHandler(const Handler& handler);
    void forward(const Event* event);

private:
    Handler mHandler = nullptr;
};

} // namespace Garbox
