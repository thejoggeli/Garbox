#include "core/rtos/TaskStopToken.h"

namespace {

Garbox::TaskStopSource source;
Garbox::TaskStopToken token = source.token();

} // namespace
