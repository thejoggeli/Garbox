#if defined(GARBOX_SIMULATION_UI)

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <lvgl.h>

#include "app/config/AppConfig.h"
#include "core/assert/AssertHandler.h"
#include "core/lvgl/LvglProvider.h"
#include "core/lvgl/LvglTheme.h"
#include "helpers/FakeTime.h"
#include "shared/types/ScreenId.h"
#include "simulation/SimulationRuntime.h"
#include "simulation/ThermalPlant.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

constexpr uint32_t Width = AppConfig::DisplayWidth;
constexpr uint32_t Height = AppConfig::DisplayHeight;
constexpr uint32_t BytesPerPixel = AppConfig::DisplayBytesPerPixel;
constexpr uint64_t TickMicros = 33'000ULL;
constexpr float TickSeconds = static_cast<float>(TickMicros) / 1'000'000.0f;

std::vector<uint8_t> Framebuffer(Width * Height * BytesPerPixel, 0);
std::vector<uint8_t> DrawBuffer(Width * Height * BytesPerPixel, 0);
bool FatalTriggered = false;

struct Options {
    const char* outputPath = nullptr;
    bool interactive = false;
};

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

std::vector<uint8_t> rgbFrame(){
    std::vector<uint8_t> rgb(Width * Height * 3U);
    for(std::size_t i = 0; i < Width * Height; ++i){
#if LV_COLOR_16_SWAP
        const uint16_t pixel = static_cast<uint16_t>(Framebuffer[i * 2] << 8U)
            | static_cast<uint16_t>(Framebuffer[i * 2 + 1]);
#else
        const uint16_t pixel = static_cast<uint16_t>(Framebuffer[i * 2])
            | static_cast<uint16_t>(Framebuffer[i * 2 + 1] << 8U);
#endif
        rgb[i * 3] = static_cast<uint8_t>(((pixel >> 11U) & 0x1fU) * 255U / 31U);
        rgb[i * 3 + 1] = static_cast<uint8_t>(((pixel >> 5U) & 0x3fU) * 255U / 63U);
        rgb[i * 3 + 2] = static_cast<uint8_t>((pixel & 0x1fU) * 255U / 31U);
    }
    return rgb;
}

bool writePpm(const char* path){
    if(!path){
        return true;
    }
    std::FILE* file = std::fopen(path, "wb");
    if(!file){
        return false;
    }
    const std::vector<uint8_t> rgb = rgbFrame();
    std::fprintf(file, "P6\n%u %u\n255\n", Width, Height);
    const bool wrote = std::fwrite(rgb.data(), rgb.size(), 1, file) == 1;
    return std::fclose(file) == 0 && wrote;
}

Options parseOptions(int argc, char** argv){
    Options options;
    if(argc == 1){
        return options;
    }
    if(argc == 2 && std::strcmp(argv[1], "--interactive") == 0){
        options.interactive = true;
        return options;
    }
    if(argc == 3 && std::strcmp(argv[1], "--output") == 0){
        options.outputPath = argv[2];
        return options;
    }
    std::fprintf(stderr, "usage: %s [--interactive | --output frame.ppm]\n", argv[0]);
    std::exit(2);
}

void stepPlant(SimulationRuntime& runtime, ThermalPlant& plant, uint32_t count){
    for(uint32_t i = 0; i < count; ++i){
        runtime.setInputs(SimulationRuntime::Inputs {
            .sensorAvailable = plant.sensorAvailable(),
            .temperatureCelcius = plant.temperatureCelcius(),
            .humidityRelative = plant.humidityRelative(),
            .fanStalled = plant.fanStalled(),
            .fanMeasuredRpm = plant.fanRpm(),
        });
        FakeTime::AdvanceMicros(TickMicros);
        runtime.step();
        const SimulationRuntime::Outputs& outputs = runtime.outputs();
        plant.setHeater(outputs.heatpad.enabled, outputs.heatpad.dutyCycle);
        plant.setFan(outputs.fan.enabled, outputs.fan.targetSpeed);
        plant.step(TickSeconds);
    }
}

int runInteractive(SimulationRuntime& runtime, ThermalPlant& plant, lv_display_t* display){
    ButtonState buttonState = ButtonState::Released;
    char command[128] {};
    std::printf("READY %u %u\n", Width, Height);
    std::fflush(stdout);

    while(std::fgets(command, sizeof(command), stdin)){
        if(std::strncmp(command, "step ", 5) == 0){
            unsigned count = 0;
            if(std::sscanf(command + 5, "%u", &count) != 1 || count > 1000U){
                std::printf("ERR invalid step count\n");
            }
            else {
                stepPlant(runtime, plant, count);
                std::printf("OK\n");
            }
        }
        else if(std::strncmp(command, "encoder ", 8) == 0){
            int steps = 0;
            if(std::sscanf(command + 8, "%d", &steps) != 1 || steps < -100 || steps > 100){
                std::printf("ERR invalid encoder step count\n");
            }
            else {
                runtime.injectEncoderSteps(steps);
                std::printf("OK\n");
            }
        }
        else if(std::strcmp(command, "button press\n") == 0){
            if(buttonState == ButtonState::Released){
                runtime.injectButton(buttonState, ButtonState::Pressed);
                buttonState = ButtonState::Pressed;
            }
            std::printf("OK\n");
        }
        else if(std::strcmp(command, "button long\n") == 0){
            if(buttonState == ButtonState::Pressed){
                runtime.injectButton(buttonState, ButtonState::PressedLong);
                buttonState = ButtonState::PressedLong;
            }
            std::printf("OK\n");
        }
        else if(std::strcmp(command, "button release\n") == 0){
            if(buttonState != ButtonState::Released){
                runtime.injectButton(buttonState, ButtonState::Released);
                buttonState = ButtonState::Released;
            }
            std::printf("OK\n");
        }
        else if(std::strcmp(command, "frame\n") == 0){
            renderFrame(runtime, display);
            const std::vector<uint8_t> rgb = rgbFrame();
            std::printf("FRAME %zu %s %.3f %.3f %u %u\n",
                rgb.size(),
                ScreenIdToString(runtime.getActiveScreenId()),
                runtime.temperatureSample().getTemperatureCelcius(),
                runtime.fermentationStatus().getTargetTemperature(),
                runtime.fermentationStatus().getPowerOn() ? 1U : 0U,
                runtime.tickCount());
            std::fwrite(rgb.data(), rgb.size(), 1, stdout);
        }
        else if(std::strcmp(command, "quit\n") == 0){
            std::printf("BYE\n");
            std::fflush(stdout);
            return FatalTriggered ? 1 : 0;
        }
        else {
            std::printf("ERR unknown command\n");
        }
        std::fflush(stdout);
        if(FatalTriggered){
            std::fprintf(stderr, "fatal application assertion in interactive simulator\n");
            return 1;
        }
    }
    return FatalTriggered ? 1 : 0;
}

} // namespace

int main(int argc, char** argv){
    const Options options = parseOptions(argc, argv);
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
    LvglTheme::Install(display);
    LvglProvider::Init(Width, Height);

    static SimulationRuntime* runtime = new SimulationRuntime();
    runtime->boot();
    ThermalPlant::Config plantConfig;
    plantConfig.initialTemperatureCelcius = 24.5f;
    plantConfig.humidityRelative = 61.0f;
    ThermalPlant plant(plantConfig);
    stepPlant(*runtime, plant, 1);

    if(options.interactive){
        const int result = runInteractive(*runtime, plant, display);
        std::fflush(nullptr);
        std::_Exit(result);
    }

    const uint64_t disabledChecksum = renderFrame(*runtime, display);

    // Exercise the same typed Button and EncoderStep events emitted by the
    // physical input controller. SimpleScreen translates these into the real
    // application requests; no simulator-only state mutations are involved.
    runtime->injectEncoderSteps(5);
    runtime->injectButton(ButtonState::Released, ButtonState::Pressed);
    runtime->injectButton(ButtonState::Pressed, ButtonState::Released);
    stepPlant(*runtime, plant, 2);

    const uint64_t enabledChecksum = renderFrame(*runtime, display);
    const bool frameChanged = std::any_of(
        Framebuffer.begin(),
        Framebuffer.end(),
        [](uint8_t value){ return value != 0; }
    );
    const bool wroteFrame = writePpm(options.outputPath);
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
        options.outputPath ? options.outputPath : "(not written)");
    if(!frameChanged){
        std::fprintf(stderr, "LVGL did not flush a non-empty frame\n");
    }
    if(!wroteFrame){
        std::fprintf(stderr, "failed to write %s\n", options.outputPath);
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
