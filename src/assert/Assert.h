#pragma once

#include <cstdint>

namespace Garbox {

void TriggerDebug(const char* context, const char* message, int32_t arg = 0);
void TriggerExit(const char* context, const char* message, int32_t arg = 0);

void AssertDebug(bool value, const char* context, const char* message, int32_t arg = 0);
void AssertExit(bool value, const char* context, const char* message, int32_t arg = 0);

}