#include "LvglLittleFs.h"

#include <lvgl.h>
#include <stdio.h>
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/fs/LittleFs.h"

namespace Garbox::LvglLittleFs {

static void * fs_open(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode){
    char full[64];
    LogDebug("fs_open", "%s", path);
    snprintf(full, sizeof(full), "/littlefs/%s", path);
    return fopen(full, (mode == LV_FS_MODE_WR ? "wb" : "rb"));
}

static lv_fs_res_t fs_write(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw){
    return LV_FS_RES_OK;
}

static lv_fs_res_t fs_close(lv_fs_drv_t *drv, void *file){
    fclose((FILE *)file);
    return LV_FS_RES_OK;
}

static lv_fs_res_t fs_read(lv_fs_drv_t *drv, void *file, void *buf, uint32_t btr, uint32_t *br){
    *br = fread(buf, 1, btr, (FILE *)file);
    return LV_FS_RES_OK;
}

static lv_fs_res_t fs_seek(lv_fs_drv_t *drv, void *file, uint32_t pos, lv_fs_whence_t whence){
    fseek(
        (FILE *)file, pos,
        whence == LV_FS_SEEK_SET ? SEEK_SET :
        whence == LV_FS_SEEK_CUR ? SEEK_CUR : SEEK_END
    );
    return LV_FS_RES_OK;
}

static lv_fs_res_t fs_tell(lv_fs_drv_t *drv, void *file, uint32_t *pos){
    *pos = ftell((FILE *)file);
    return LV_FS_RES_OK;
}

static bool sInitialized = false;
static char sDriveLetter = '\0';

void Init(char driveLetter){

    AssertExit(!sInitialized, "LvglLittleFs", "already initialized");


    if(!LittleFs::IsInitialized()){
        LittleFs::Init();
    }

    LogDebug("LvglLittleFs", "init, drive=%c", driveLetter);

    static lv_fs_drv_t drv;
    lv_fs_drv_init(&drv);

    drv.letter = driveLetter;
    drv.user_data = nullptr;
    drv.open_cb = fs_open;
    drv.write_cb = fs_write;
    drv.close_cb = fs_close;
    drv.read_cb = fs_read;
    drv.seek_cb = fs_seek;
    drv.tell_cb = fs_tell;
    drv.dir_open_cb = nullptr;
    drv.dir_read_cb = nullptr;
    drv.dir_close_cb = nullptr;

    lv_fs_drv_register(&drv);

    sDriveLetter = driveLetter;
    sInitialized = true;
}

bool IsInitialized(){
    return sInitialized;
}

char GetDriveLetter(){
    AssertExit(sInitialized, "LvglLittleFs", "not initialized");
    return sDriveLetter;
}

} // namespace