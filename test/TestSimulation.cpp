#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>

#include <unity.h>

#include "helpers/AssertHelper.h"
#include "helpers/FakeTime.h"
#include "simulation/SimulationRuntime.h"
#include "simulation/ThermalPlant.h"

using namespace Garbox;
using namespace GarboxTest;

namespace {

constexpr uint64_t TickMicros = 33'000ULL;

SimulationRuntime& makeRuntime(){
    // Runtime/container destruction is tracked by LIFE-01. Keep test instances
    // process-owned until that contract is converted to RAII.
    static SimulationRuntime* runtimes[12] {};
    static std::size_t runtimeCount = 0;
    TEST_ASSERT_LESS_THAN_UINT32(12U, runtimeCount);

    FakeTime::Reset();
    auto* runtime = new SimulationRuntime();
    runtimes[runtimeCount++] = runtime;
    runtime->boot();
    return *runtime;
}

void step(SimulationRuntime& runtime, uint32_t count = 1){
    for(uint32_t i = 0; i < count; ++i){
        FakeTime::AdvanceMicros(TickMicros);
        runtime.step();
    }
}

SimulationRuntime::Inputs validInputs(float temperatureCelcius){
    SimulationRuntime::Inputs inputs;
    inputs.sensorAvailable = true;
    inputs.temperatureCelcius = temperatureCelcius;
    inputs.humidityRelative = 60.0f;
    inputs.fanStalled = false;
    inputs.fanMeasuredRpm = 1200.0f;
    return inputs;
}

void assertBoundedOutputs(const SimulationRuntime::Outputs& outputs){
    TEST_ASSERT_TRUE(std::isfinite(outputs.heatpad.dutyCycle));
    TEST_ASSERT_TRUE(std::isfinite(outputs.fan.targetSpeed));
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, outputs.heatpad.dutyCycle);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, outputs.heatpad.dutyCycle);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, outputs.fan.targetSpeed);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, outputs.fan.targetSpeed);
}

void test_simulation_boots_disabled_and_safe(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();

    TEST_ASSERT_FALSE(runtime.fermentationStatus().getPowerOn());
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::Reset),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, runtime.outputs().heatpad.dutyCycle);
    TEST_ASSERT_FALSE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, runtime.outputs().fan.targetSpeed);
    TEST_ASSERT_EQUAL_UINT32(1U, runtime.tickCount());
    assertBoundedOutputs(runtime.outputs());
    TEST_ASSERT_NO_ASSERTS();
}

void test_simulation_valid_cold_input_enters_regulation(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestTargetTemperature(32.0f);
    runtime.requestFermentationEnabled(true);

    step(runtime);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::Regulating),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.4f, runtime.outputs().fan.targetSpeed);

    step(runtime);
    TEST_ASSERT_TRUE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, runtime.outputs().heatpad.dutyCycle);
    assertBoundedOutputs(runtime.outputs());
    TEST_ASSERT_NO_ASSERTS();
}

void test_simulation_overtemperature_forces_heat_off_and_fan_full(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestTargetTemperature(32.0f);
    runtime.requestFermentationEnabled(true);
    step(runtime, 2);
    TEST_ASSERT_TRUE(runtime.outputs().heatpad.enabled);

    runtime.setInputs(validInputs(35.0f));
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::OverTemperature),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, runtime.outputs().heatpad.dutyCycle);
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, runtime.outputs().fan.targetSpeed);
    assertBoundedOutputs(runtime.outputs());
    TEST_ASSERT_NO_ASSERTS();
}

void test_simulation_sensor_loss_and_fan_stall_force_safe_commands(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestFermentationEnabled(true);
    step(runtime, 2);

    SimulationRuntime::Inputs failed = validInputs(20.0f);
    failed.sensorAvailable = false;
    runtime.setInputs(failed);
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::InvalidInput),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, runtime.outputs().fan.targetSpeed);

    failed.sensorAvailable = true;
    failed.fanStalled = true;
    runtime.setInputs(failed);
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::InvalidInput),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, runtime.outputs().fan.targetSpeed);
    assertBoundedOutputs(runtime.outputs());
    TEST_ASSERT_NO_ASSERTS();
}

void test_simulation_hot_disable_cools_down_before_reset(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(35.0f));
    runtime.requestFermentationEnabled(true);
    step(runtime);

    runtime.requestFermentationEnabled(false);
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::Cooldown),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, runtime.outputs().fan.targetSpeed);

    runtime.setInputs(validInputs(29.0f));
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::Reset),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_FALSE(runtime.outputs().fan.enabled);
    assertBoundedOutputs(runtime.outputs());
    TEST_ASSERT_NO_ASSERTS();
}

void test_simulation_safety_fan_overrides_manual_off_preference(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestFermentationEnabled(true);
    runtime.requestAutomaticFan(false);
    step(runtime, 2);

    TEST_ASSERT_FALSE(runtime.outputs().fan.enabled);

    runtime.setInputs(validInputs(35.0f));
    step(runtime);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::OverTemperature),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, runtime.outputs().fan.targetSpeed);
    TEST_ASSERT_NO_ASSERTS();
}

void test_simulation_manual_fan_speed_is_independent_of_regulation_power(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestFanControl(false, 0.35f);
    step(runtime);

    TEST_ASSERT_FALSE(runtime.fermentationStatus().getPowerOn());
    TEST_ASSERT_FALSE(runtime.fermentationStatus().getFanAuto());
    TEST_ASSERT_FLOAT_WITHIN(
        0.0001f, 0.35f, runtime.fermentationStatus().getFanManualSpeed());
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.35f, runtime.outputs().fan.targetSpeed);

    runtime.requestAutomaticFan(true);
    step(runtime);

    TEST_ASSERT_TRUE(runtime.fermentationStatus().getFanAuto());
    TEST_ASSERT_FALSE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, runtime.outputs().fan.targetSpeed);
    TEST_ASSERT_NO_ASSERTS();
}

void test_closed_loop_plant_runs_faster_than_real_time_with_safe_outputs(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    ThermalPlant plant;
    runtime.requestTargetTemperature(25.0f);
    runtime.requestFermentationEnabled(true);

    constexpr uint32_t fiveMinutesInTicks = (5U * 60U * 1'000U) / 33U;
    for(uint32_t i = 0; i < fiveMinutesInTicks; ++i){
        runtime.setInputs(SimulationRuntime::Inputs {
            .sensorAvailable = plant.sensorAvailable(),
            .temperatureCelcius = plant.temperatureCelcius(),
            .humidityRelative = plant.humidityRelative(),
            .fanStalled = plant.fanStalled(),
            .fanMeasuredRpm = plant.fanRpm(),
        });
        step(runtime);

        const auto& outputs = runtime.outputs();
        plant.setHeater(outputs.heatpad.enabled, outputs.heatpad.dutyCycle);
        plant.setFan(outputs.fan.enabled, outputs.fan.targetSpeed);
        plant.step(0.033f);

        assertBoundedOutputs(outputs);
        const FermentationState state = runtime.fermentationStatus().getState();
        const bool heatMustBeOff = state == FermentationState::Reset
            || state == FermentationState::Ready
            || state == FermentationState::InvalidInput
            || state == FermentationState::OverTemperature
            || state == FermentationState::Cooldown;
        if(heatMustBeOff){
            TEST_ASSERT_FALSE(outputs.heatpad.enabled);
            TEST_ASSERT_EQUAL_FLOAT(0.0f, outputs.heatpad.dutyCycle);
        }
    }

    TEST_ASSERT_GREATER_THAN_FLOAT(20.1f, plant.temperatureCelcius());
    TEST_ASSERT_GREATER_THAN_UINT32(9'000U, runtime.tickCount());
    TEST_ASSERT_NO_ASSERTS();
}

void test_stale_temperature_sample_forces_safe_commands(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestFermentationEnabled(true);
    step(runtime, 2);
    TEST_ASSERT_TRUE(runtime.outputs().heatpad.enabled);

    SimulationRuntime::Inputs stale = validInputs(20.0f);
    stale.sensorSampleUpdated = false;
    runtime.setInputs(stale);
    step(runtime, 92);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::InvalidInput),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, runtime.outputs().heatpad.dutyCycle);
    TEST_ASSERT_TRUE(runtime.outputs().fan.enabled);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, runtime.outputs().fan.targetSpeed);
    TEST_ASSERT_NO_ASSERTS();
}

void test_nonfinite_and_implausible_sensor_values_are_rejected(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    runtime.setInputs(validInputs(20.0f));
    runtime.requestFermentationEnabled(true);
    step(runtime, 2);

    SimulationRuntime::Inputs bad = validInputs(20.0f);
    bad.temperatureCelcius = std::numeric_limits<float>::quiet_NaN();
    runtime.setInputs(bad);
    step(runtime);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::InvalidInput),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);

    bad = validInputs(20.0f);
    bad.humidityRelative = 120.0f;
    runtime.setInputs(bad);
    step(runtime);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(FermentationState::InvalidInput),
                            static_cast<uint8_t>(runtime.fermentationStatus().getState()));
    TEST_ASSERT_FALSE(runtime.outputs().heatpad.enabled);
    TEST_ASSERT_NO_ASSERTS();
}

void test_target_requests_are_finite_and_bounded(){
    TEST_RESET_ASSERTS();
    SimulationRuntime& runtime = makeRuntime();
    const float initial = runtime.fermentationStatus().getTargetTemperature();

    runtime.requestTargetTemperature(std::numeric_limits<float>::quiet_NaN());
    TEST_ASSERT_EQUAL_FLOAT(initial, runtime.fermentationStatus().getTargetTemperature());
    TEST_ASSERT_EQUAL_UINT32(1, AssertDebugCount);

    runtime.requestTargetTemperature(100.0f);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, runtime.fermentationStatus().getTargetTemperature());
}

} // namespace

void RegisterSimulationTests(){
    RUN_TEST(test_simulation_boots_disabled_and_safe);
    RUN_TEST(test_simulation_valid_cold_input_enters_regulation);
    RUN_TEST(test_simulation_overtemperature_forces_heat_off_and_fan_full);
    RUN_TEST(test_simulation_sensor_loss_and_fan_stall_force_safe_commands);
    RUN_TEST(test_simulation_hot_disable_cools_down_before_reset);
    RUN_TEST(test_simulation_safety_fan_overrides_manual_off_preference);
    RUN_TEST(test_simulation_manual_fan_speed_is_independent_of_regulation_power);
    RUN_TEST(test_closed_loop_plant_runs_faster_than_real_time_with_safe_outputs);
    RUN_TEST(test_stale_temperature_sample_forces_safe_commands);
    RUN_TEST(test_nonfinite_and_implausible_sensor_values_are_rejected);
    RUN_TEST(test_target_requests_are_finite_and_bounded);
}
