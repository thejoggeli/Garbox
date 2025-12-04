#include "LittleFs.h"

#include <esp_littlefs.h>
#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox::LittleFs {

static bool sInitialized = false;

void Init(){
    AssertExit(!sInitialized, "LvglLittleFs", "already initialized");

    LogDebug("LittleFs", "init");

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .partition = nullptr, // use default partition by label
        .format_if_mount_failed = true,
        .read_only = false,
        .dont_mount = false,
        .grow_on_mount = false
    };
    esp_err_t err = esp_vfs_littlefs_register(&conf);
    
    AssertExit(err == ESP_OK, "LvglLittleFs", "vfs_littlefs_register failed", static_cast<int32_t>(err));

    sInitialized = true;
}

bool IsInitialized(){
    return sInitialized;
}

} // namespace