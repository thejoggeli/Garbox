#pragma once

#include <cstdarg>

typedef enum {
    ESP_LOG_NONE,
    ESP_LOG_ERROR,
    ESP_LOG_WARN,
    ESP_LOG_INFO,
    ESP_LOG_DEBUG,
    ESP_LOG_VERBOSE,
} esp_log_level_t;

inline void esp_log_write(esp_log_level_t, const char*, const char*, ...){}
