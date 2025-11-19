#pragma once

#include <cstdarg>
#include <cinttypes>
#include "esp_log.h"

namespace Garbox {

#define LOG_OUTPUT_TAGS 1

#if LOG_OUTPUT_TAGS
    #define LogError(tag, fmt, ...)   esp_log_write(ESP_LOG_ERROR,   tag, "[ERROR..] (%s) " fmt "\n", tag, ##__VA_ARGS__)
    #define LogWarning(tag, fmt, ...) esp_log_write(ESP_LOG_WARN,    tag, "[WARNING] (%s) " fmt "\n", tag, ##__VA_ARGS__)
    #define LogInfo(tag, fmt, ...)    esp_log_write(ESP_LOG_INFO,    tag, "[INFO...] (%s) " fmt "\n", tag, ##__VA_ARGS__)
    #define LogDebug(tag, fmt, ...)   esp_log_write(ESP_LOG_DEBUG,   tag, "[DEBUG..] (%s) " fmt "\n", tag, ##__VA_ARGS__)
    #define LogVerbose(tag, fmt, ...) esp_log_write(ESP_LOG_VERBOSE, tag, "[VERBOSE] (%s) " fmt "\n", tag, ##__VA_ARGS__)
    
    #define LogError0(tag, fmt, ...)   esp_log_write(ESP_LOG_ERROR,   tag, "[ERROR..] (%s) " fmt, tag, ##__VA_ARGS__)
    #define LogWarning0(tag, fmt, ...) esp_log_write(ESP_LOG_WARN,    tag, "[WARNING] (%s) " fmt, tag, ##__VA_ARGS__)
    #define LogInfo0(tag, fmt, ...)    esp_log_write(ESP_LOG_INFO,    tag, "[INFO...] (%s) " fmt, tag, ##__VA_ARGS__)
    #define LogDebug0(tag, fmt, ...)   esp_log_write(ESP_LOG_DEBUG,   tag, "[DEBUG..] (%s) " fmt, tag, ##__VA_ARGS__)
    #define LogVerbose0(tag, fmt, ...) esp_log_write(ESP_LOG_VERBOSE, tag, "[VERBOSE] (%s) " fmt, tag, ##__VA_ARGS__)
#else 
    #define LogInfo(tag, fmt, ...)    esp_log_write(ESP_LOG_INFO, tag, fmt "\n", ##__VA_ARGS__)
    #define LogWarning(tag, fmt, ...) esp_log_write(ESP_LOG_WARN, tag, fmt "\n", ##__VA_ARGS__)
    #define LogError(tag, fmt, ...)   esp_log_write(ESP_LOG_ERROR, tag, fmt "\n", ##__VA_ARGS__)
    #define LogDebug(tag, fmt, ...)   esp_log_write(ESP_LOG_DEBUG, tag, fmt "\n", ##__VA_ARGS__)
    #define LogVerbose(tag, fmt, ...) esp_log_write(ESP_LOG_VERBOSE, tag, fmt "\n", ##__VA_ARGS__)
#endif

enum class LogLevel : uint8_t {
    None    = ESP_LOG_NONE,
    Error   = ESP_LOG_ERROR,
    Warning = ESP_LOG_WARN,
    Info    = ESP_LOG_INFO,
    Debug   = ESP_LOG_DEBUG,
    Verbose = ESP_LOG_VERBOSE
};

class Log {
public:
    static void Init();
    static void SetLevel(LogLevel level, const char* tag = "*");
};

}