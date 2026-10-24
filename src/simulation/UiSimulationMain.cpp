#if defined(GARBOX_SIMULATION_UI)

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <lvgl.h>

#include "core/assert/AssertHandler.h"
#include "core/lvgl/LvglProvider.h"
#include "helpers/FakeTime.h"
#include "simulation/SimulationRuntime.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

constexpr uint32_t Width = 320;
constexpr uint32_t Height = 240;
constexpr uint32_t BytesPerPixel = 2;
constexpr uint64_t TickMicros = 33'000ULL;

std::vector<uint8_t> Framebuffer(Width * Height * BytesPerPixel, 0);
std::vector<uint8_t> DrawBuffer(Width * Height * BytesPerPixel, 0);
bool FatalTriggered = false;

uint32_t tickProvider(){
    return Time::GetMillis();
}

void flushDisplay(lv_display_t* display, const lv_area_t* area, uint8_t* pixels){
    const int32_t areaWidth = area->x2 - area->x1 + 1;
    const int32_t areaHeight = area->y2 - area->y1 + 1;
    const std::size_t sourceStride = static_cast<std::size_t>(areaWidth) * BytesPerPixel;
    const std::size_t targetStride = static_cast<std::size_t>(Width) * BytesPerPixel;

    for(int32_t row = 0; row < areaHeight; ++row){
        const std::size_t sourceOffset = static_cast<std::size_t>(row) * sourceStride;
        const std::size_t targetOffset =
            (static_cast<std::size_t>(area->y1 + row) * targetStride)
            + static_cast<std::size_t>(area->x1) * BytesPerPixel;
        std::memcpy(Framebuffer.data() + targetOffset, pixels + sourceOffset, sourceStride);
    }
    lv_display_flush_ready(display);
}

uint64_t frameChecksum(){
    uint64_t value = 1469598103934665603ULL;
    for(uint8_t byte : Framebuffer){
        value ^= byte;
        value *= 1099511628211ULL;
    }
    return value;
}

uint64_t renderFrame(SimulationRuntime& runtime, lv_display_t* display){
    runtime.render();
    lv_obj_invalidate(lv_screen_active());
    lv_refr_now(display);
    return frameChecksum();
}

bool writePpm(const char* path){
    if(!path){
        return true;
    }
    std::FILE* file = std::fopen(path, "wb");
    if(!file){
        return false;
    }
    std::fprintf(file, "P6\n%u %u\n255\n", Width, Height);
    for(std::size_t i = 0; i < Width * Height; ++i){
#if LV_COLOR_16_SWAP
        const uint16_t pixel = static_cast<uint16_t>(Framebuffer[i * 2] << 8U)
            | static_cast<uint16_t>(Framebuffer[i * 2 + 1]);
#else
        const uint16_t pixel = static_cast<uint16_t>(Framebuffer[i * 2])
            | static_cast<uint16_t>(Framebuffer[i * 2 + 1] << 8U);
#endif
        const uint8_t rgb[3] = {
            static_cast<uint8_t>(((pixel >> 11U) & 0x1fU) * 255U / 31U),
            static_cast<uint8_t>(((pixel >> 5U) & 0x3fU) * 255U / 63U),
            static_cast<uint8_t>((pixel & 0x1fU) * 255U / 31U),
        };
        std::fwrite(rgb, sizeof(rgb), 1, file);
    }
    return std::fclose(file) == 0;
}

const char* outputPath(int argc, char** argv){
    if(argc == 1){
        return nullptr;
    }
    if(argc == 3 && std::strcmp(argv[1], "--output") == 0){
        return argv[2];
    }
    std::fprintf(stderr, "usage: %s [--output frame.ppm]\n", argv[0]);
    std::exit(2);
}

} // namespace

int main(int argc, char** argv){
    const char* path = outputPath(argc, argv);
    AssertHandler::SetDebugHandler([](const char*, const char*, int32_t){});
    AssertHandler::SetExitHandler([](const char*, const char*, int32_t){
        FatalTriggered = true;
    });
    FakeTime::Reset();

    lv_init();
    lv_tick_set_cb(tickProvider);
    lv_display_t* display = lv_display_create(Width, Height);
    if(!display){
        std::fprintf(stderr, "failed to create LVGL display\n");
        return 1;
    }
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_buffers(
        display,
        DrawBuffer.data(),
        nullptr,
        static_cast<uint32_t>(DrawBuffer.size()),
        LV_DISPLAY_RENDER_MODE_FULL
    );
    lv_display_set_flush_cb(display, flushDisplay);
    LvglProvider::Init(Width, Height);

    static SimulationRuntime* runtime = new SimulationRuntime();
    runtime->boot();
    runtime->setInputs(SimulationRuntime::Inputs {
        .sensorAvailable = true,
        .temperatureCelcius = 24.5f,
        .humidityRelative = 61.0f,
        .fanStalled = false,
        .fanMeasuredRpm = 960.0f,
    });

    FakeTime::AdvanceMicros(TickMicros);
    runtime->step();
    const uint64_t disabledChecksum = renderFrame(*runtime, display);

    // Exercise the same typed Button and EncoderStep events emitted by the
    // physical input controller. SimpleScreen translates these into the real
    // application requests; no simulator-only state mutations are involved.
    runtime->injectEncoderSteps(5);
    runtime->injectButton(ButtonState::Released, ButtonState::Pressed);
    runtime->injectButton(ButtonState::Pressed, ButtonState::Released);

    for(uint32_t i = 0; i < 2; ++i){
        FakeTime::AdvanceMicros(TickMicros);
        runtime->step();
    }
    const uint64_t enabledChecksum = renderFrame(*runtime, display);
    const bool frameChanged = std::any_of(
        Framebuffer.begin(),
        Framebuffer.end(),
        [](uint8_t value){ return value != 0; }
    );
    const bool wroteFrame = writePpm(path);
    const bool inputReachedApplication = runtime->fermentationStatus().getPowerOn()
        && runtime->fermentationStatus().getTargetTemperature() == 32.5f;
    const bool frameResponded = disabledChecksum != enabledChecksum;
    const bool ok = !FatalTriggered
        && frameChanged
        && frameResponded
        && inputReachedApplication
        && wroteFrame;

    std::printf("ui=Simple size=%ux%u disabled=%016llx enabled=%016llx "
                "power=%s target=%.1f output=%s\n",
        Width,
        Height,
        static_cast<unsigned long long>(disabledChecksum),
        static_cast<unsigned long long>(enabledChecksum),
        runtime->fermentationStatus().getPowerOn() ? "on" : "off",
        runtime->fermentationStatus().getTargetTemperature(),
        path ? path : "(not written)");
    if(!frameChanged){
        std::fprintf(stderr, "LVGL did not flush a non-empty frame\n");
    }
    if(!wroteFrame){
        std::fprintf(stderr, "failed to write %s\n", path);
    }
    if(!frameResponded){
        std::fprintf(stderr, "LVGL frame did not respond to simulated input\n");
    }
    if(!inputReachedApplication){
        std::fprintf(stderr, "simulated input did not reach application state\n");
    }

    // Several existing framework objects intentionally have fatal destructors
    // until LIFE-01 converts them to RAII. Keep the simulator process-owned,
    // exactly like the firmware lifetime, and avoid misleading teardown noise.
    std::fflush(nullptr);
    std::_Exit(ok ? 0 : 1);
}

#endif
