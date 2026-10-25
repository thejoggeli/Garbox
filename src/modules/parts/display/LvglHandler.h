#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <lvgl.h>
#include "core/util/math/RectXXYY.h"

namespace Garbox {

class LvglHandler {
public:

    static constexpr std::size_t DrawBufferCount = 3U;

    using FlushHandler = std::function<void(const RectXXYY<int32_t> area, uint8_t* buffer)>;
    using FlushWaitHandler = std::function<void()>;

    struct Config {
        uint32_t width;
        uint32_t height;
        uint32_t bytesPerPixel;
        uint32_t partialFactor;
    };

    LvglHandler(const Config& config);
    ~LvglHandler();

    void init();
    void startRender();

    void setFlushHandler(FlushHandler handler);
    void setFlushWaitHandler(FlushWaitHandler handler);
    std::array<const uint8_t*, DrawBufferCount> getDrawBufferPointers() const;

private: 

    bool mInitialized = false;

    // lvgl
    lv_display_t* mLvDisplay = nullptr;

    // flush handling
    FlushHandler mFlushHandler = nullptr;
    FlushWaitHandler mFlushWaitHandler = nullptr;

    // drawing buffer
    lv_draw_buf_t mDrawBuffer1;
    lv_draw_buf_t mDrawBuffer2;
    lv_draw_buf_t mDrawBuffer3;
    uint8_t* mDrawBufferData1 = nullptr;
    uint8_t* mDrawBufferData2 = nullptr;
    uint8_t* mDrawBufferData3 = nullptr;
 
    const uint32_t mWidth;
    const uint32_t mHeight;
    const uint32_t mBytesPerPixel;
    const uint32_t mPartialFactor;
    const uint32_t mBufferWidth;
    const uint32_t mBufferHeight;
    const uint32_t mBufferStride;
    const uint32_t mBufferSize;

    // trampolines
    static void handleFlush(lv_display_t* disp, const lv_area_t* area, uint8_t* pixelMap);
    static void handleFlushWait(lv_display_t* disp);

    // lvgl helpers
    static void handleLog(lv_log_level_t level, const char* buf);
    static uint32_t lvglTickProvider();

    // buffer helpers
    static uint8_t* allocDrawBufferData(uint32_t size);
    static void initDrawBuffer(lv_draw_buf_t& buffer, uint8_t* data, uint32_t size,
                               uint32_t width, uint32_t height, uint32_t stride);

};

} // namespace
