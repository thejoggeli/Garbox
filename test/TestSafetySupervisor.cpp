#include <unity.h>

#include "app/safety/FermentationSafetySupervisor.h"

using namespace Garbox;

namespace {

FermentationSafetySupervisor::Input validInput(uint64_t nowMicros = 1'000'000ULL){
    return FermentationSafetySupervisor::Input {
        .regulationRequested = true,
        .sensorAvailable = true,
        .sampleMicros = nowMicros,
        .temperatureCelcius = 25.0f,
        .humidityRelative = 60.0f,
        .heaterMeasuredVoltage = 0.0f,
        .heaterMeasuredCurrent = 0.0f,
        .nowMicros = nowMicros,
    };
}

void test_sensor_interlocks_recover_but_never_permit_unsafe_heat(){
    FermentationSafetySupervisor supervisor;
    auto input = validInput(4'000'001ULL);
    input.sampleMicros = 1'000'000ULL;
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::SensorStale),
                            static_cast<uint8_t>(supervisor.fault()));
    TEST_ASSERT_FALSE(supervisor.sensorValid());
    TEST_ASSERT_FALSE(supervisor.permitHeater(true, 0.5f));

    input = validInput(5'000'000ULL);
    input.temperatureCelcius = -30.0f;
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::SensorImplausible),
                            static_cast<uint8_t>(supervisor.fault()));

    input = validInput(6'000'000ULL);
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::None),
                            static_cast<uint8_t>(supervisor.fault()));
    TEST_ASSERT_TRUE(supervisor.permitHeater(true, 0.5f));
}

void test_absolute_temperature_fault_latches_across_recovery(){
    FermentationSafetySupervisor supervisor;
    auto input = validInput();
    input.temperatureCelcius = 60.0f;
    supervisor.update(input);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::AbsoluteOverTemperature),
                            static_cast<uint8_t>(supervisor.latchedFault()));
    TEST_ASSERT_FALSE(supervisor.permitHeater(true, 0.5f));
    TEST_ASSERT_TRUE(supervisor.fanRequired());

    input = validInput(2'000'000ULL);
    input.regulationRequested = false;
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::AbsoluteOverTemperature),
                            static_cast<uint8_t>(supervisor.fault()));
    TEST_ASSERT_FALSE(supervisor.permitHeater(true, 0.5f));
}

void test_unexpected_heater_measurement_must_persist_then_latches(){
    FermentationSafetySupervisor supervisor;
    auto input = validInput(1'000'000ULL);
    input.regulationRequested = false;
    input.heaterMeasuredVoltage = 2.0f;
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::None),
                            static_cast<uint8_t>(supervisor.fault()));

    input.nowMicros += FermentationSafetySupervisor::UnexpectedHeaterPersistenceMicros - 1U;
    input.sampleMicros = input.nowMicros;
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::None),
                            static_cast<uint8_t>(supervisor.fault()));

    input.nowMicros += 1U;
    input.sampleMicros = input.nowMicros;
    supervisor.update(input);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::UnexpectedHeaterOutput),
                            static_cast<uint8_t>(supervisor.latchedFault()));
    TEST_ASSERT_TRUE(supervisor.fanRequired());
}

void test_invalid_heater_command_is_vetoed(){
    FermentationSafetySupervisor supervisor;
    supervisor.update(validInput());
    TEST_ASSERT_FALSE(supervisor.permitHeater(true, 1.1f));
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SafetyFault::InvalidHeaterCommand),
                            static_cast<uint8_t>(supervisor.fault()));
}

} // namespace

void RegisterSafetySupervisorTests(){
    RUN_TEST(test_sensor_interlocks_recover_but_never_permit_unsafe_heat);
    RUN_TEST(test_absolute_temperature_fault_latches_across_recovery);
    RUN_TEST(test_unexpected_heater_measurement_must_persist_then_latches);
    RUN_TEST(test_invalid_heater_command_is_vetoed);
}
