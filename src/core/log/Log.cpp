#include "Log.h"

#include "assert/Assert.h"

namespace Garbox {

static bool mInitialized = false;

void Log::Init(){

    if(mInitialized){
        AssertExit(false, "Log::Init()", "already initialized");
        return;
    }

    esp_log_level_set("*", ESP_LOG_VERBOSE);

    mInitialized = true;
}

void Log::SetLevel(Level level, const char* tag) {
    esp_log_level_set(tag, static_cast<esp_log_level_t>(level));
}

}