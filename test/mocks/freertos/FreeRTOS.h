#pragma once

#include <cstdint>

using BaseType_t = int32_t;
using UBaseType_t = uint32_t;
using TickType_t = uint32_t;

inline constexpr BaseType_t pdFALSE = 0;
inline constexpr BaseType_t pdTRUE = 1;
inline constexpr BaseType_t pdPASS = 1;
inline constexpr TickType_t portMAX_DELAY = UINT32_MAX;

#define pdMS_TO_TICKS(milliseconds) static_cast<TickType_t>(milliseconds)
