#if defined(GARBOX_SIMULATOR_MAIN) && !defined(GARBOX_SIMULATION_UI)

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "helpers/FakeTime.h"
#include "core/assert/AssertHandler.h"
#include "simulation/SimulationRuntime.h"
#include "simulation/ThermalPlant.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

constexpr uint64_t TickMicros = 33'000ULL;
constexpr float TickSeconds = 0.033f;
bool FatalTriggered = false;

struct Options {
    uint32_t minutes = 10;
    float targetCelcius = 32.0f;
    float initialCelcius = 20.0f;
};

Options parseOptions(int argc, char** argv){
    Options options;
    for(int i = 1; i < argc; ++i){
        if(std::strcmp(argv[i], "--minutes") == 0 && i + 1 < argc){
            options.minutes = static_cast<uint32_t>(std::strtoul(argv[++i], nullptr, 10));
        }
        else if(std::strcmp(argv[i], "--target") == 0 && i + 1 < argc){
            options.targetCelcius = std::strtof(argv[++i], nullptr);
        }
        else if(std::strcmp(argv[i], "--initial") == 0 && i + 1 < argc){
            options.initialCelcius = std::strtof(argv[++i], nullptr);
        }
        else {
            std::fprintf(stderr, "usage: %s [--minutes N] [--target C] [--initial C]\n", argv[0]);
            std::exit(2);
        }
    }
    return options;
}

bool outputsAreSafe(const SimulationRuntime& runtime){
    const auto state = runtime.fermentationStatus().getState();
    const auto& outputs = runtime.outputs();
    const bool forcedOff = state == FermentationState::InvalidInput
        || state == FermentationState::OverTemperature
        || state == FermentationState::Cooldown
        || state == FermentationState::Reset
        || state == FermentationState::Ready;
    return std::isfinite(outputs.heatpad.dutyCycle)
        && outputs.heatpad.dutyCycle >= 0.0f
        && outputs.heatpad.dutyCycle <= 1.0f
        && (!forcedOff || (!outputs.heatpad.enabled && outputs.heatpad.dutyCycle == 0.0f));
}

} // namespace

int main(int argc, char** argv){
    const Options options = parseOptions(argc, argv);
    AssertHandler::SetDebugHandler([](const char*, const char*, int32_t){});
    AssertHandler::SetExitHandler([](const char*, const char*, int32_t){
        FatalTriggered = true;
    });
    FakeTime::Reset();

    static SimulationRuntime* runtime = new SimulationRuntime();
    ThermalPlant plant(ThermalPlant::Config {
        .initialTemperatureCelcius = options.initialCelcius,
    });

    runtime->boot();
    runtime->requestTargetTemperature(options.targetCelcius);
    runtime->requestFermentationEnabled(true);

    std::puts("seconds,temperature,state,heater_enabled,heater_duty,fan_enabled,fan_speed,fan_rpm");
    const uint64_t totalTicks = static_cast<uint64_t>(options.minutes) * 60'000ULL / 33ULL;
    for(uint64_t tick = 0; tick < totalTicks; ++tick){
        runtime->setInputs(SimulationRuntime::Inputs {
            .sensorAvailable = plant.sensorAvailable(),
            .temperatureCelcius = plant.temperatureCelcius(),
            .humidityRelative = plant.humidityRelative(),
            .fanStalled = plant.fanStalled(),
            .fanMeasuredRpm = plant.fanRpm(),
        });

        FakeTime::AdvanceMicros(TickMicros);
        runtime->step();

        const auto& outputs = runtime->outputs();
        plant.setHeater(outputs.heatpad.enabled, outputs.heatpad.dutyCycle);
        plant.setFan(outputs.fan.enabled, outputs.fan.targetSpeed);
        plant.step(TickSeconds);

        if(!outputsAreSafe(*runtime) || FatalTriggered){
            std::fprintf(stderr, "simulation invariant failed at tick %llu\n",
                         static_cast<unsigned long long>(tick));
            return 1;
        }

        if(tick % 30ULL == 0){
            std::printf("%.3f,%.3f,%s,%u,%.5f,%u,%.3f,%.1f\n",
                static_cast<double>(tick) * TickSeconds,
                plant.temperatureCelcius(),
                FermentationStateToString(runtime->fermentationStatus().getState()),
                outputs.heatpad.enabled ? 1U : 0U,
                outputs.heatpad.dutyCycle,
                outputs.fan.enabled ? 1U : 0U,
                outputs.fan.targetSpeed,
                plant.fanRpm());
        }
    }

    return 0;
}

#endif
