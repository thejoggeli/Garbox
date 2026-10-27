#pragma once

#include <cstdint>

#include "app/engine/FermentationState.h"

namespace Garbox {

/**
 * Final heater-permission layer, independent of the normal PID/FSM policy.
 * Absolute temperature and unexpected electrical output faults latch until
 * process restart. Sensor faults recover automatically once input is valid.
 */
class FermentationSafetySupervisor {
public:
    struct Input {
        bool regulationRequested = false;
        bool sensorAvailable = false;
        uint64_t sampleMicros = 0;
        float temperatureCelcius = 0.0f;
        float humidityRelative = 0.0f;
        float heaterMeasuredVoltage = 0.0f;
        float heaterMeasuredCurrent = 0.0f;
        uint64_t nowMicros = 0;
    };

    static constexpr uint64_t MaxSampleAgeMicros = 3'000'000ULL;
    static constexpr float MinPlausibleTemperatureCelcius = -20.0f;
    static constexpr float MaxPlausibleTemperatureCelcius = 80.0f;
    static constexpr float MinPlausibleHumidityRelative = 0.0f;
    static constexpr float MaxPlausibleHumidityRelative = 100.0f;
    static constexpr float AbsoluteTemperatureLimitCelcius = 60.0f;

    // Provisional until calibrated on hardware. The persistence delay avoids
    // latching on ADC/filter decay immediately after a legitimate off command.
    static constexpr float UnexpectedHeaterVoltageThreshold = 1.0f;
    static constexpr float UnexpectedHeaterCurrentThreshold = 0.1f;
    static constexpr uint64_t UnexpectedHeaterPersistenceMicros = 2'000'000ULL;

    void update(const Input& input);
    bool permitHeater(bool requestedEnabled, float requestedDuty);
    void recordFinalHeaterCommand(bool enabled, float duty);

    bool sensorValid() const { return mSensorValid; }
    bool fanRequired() const { return mActiveFault != SafetyFault::None; }
    SafetyFault fault() const { return mActiveFault; }
    SafetyFault latchedFault() const { return mLatchedFault; }

private:
    void latch(SafetyFault fault);

    bool mSensorValid = false;
    bool mLastHeaterCommandedOn = false;
    bool mUnexpectedOutputTiming = false;
    uint64_t mUnexpectedOutputStartMicros = 0;
    SafetyFault mTransientFault = SafetyFault::None;
    SafetyFault mLatchedFault = SafetyFault::None;
    SafetyFault mActiveFault = SafetyFault::None;
};

} // namespace Garbox
