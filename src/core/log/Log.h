#pragma once

#include "esp_log.h"
#include <cstdarg>
#include <cinttypes>

namespace Garbox {

#define LogInfo(tag, fmt, ...)    esp_log_write(ESP_LOG_INFO, tag, fmt "\n", ##__VA_ARGS__)
#define LogWarning(tag, fmt, ...)    esp_log_write(ESP_LOG_WARN, tag, fmt "\n", ##__VA_ARGS__)
#define LogError(tag, fmt, ...)   esp_log_write(ESP_LOG_ERROR, tag, fmt "\n", ##__VA_ARGS__)
#define LogDebug(tag, fmt, ...)   esp_log_write(ESP_LOG_DEBUG, tag, fmt "\n", ##__VA_ARGS__)
#define LogVerbose(tag, fmt, ...) esp_log_write(ESP_LOG_VERBOSE, tag, fmt "\n", ##__VA_ARGS__)

}