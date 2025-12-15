#pragma once

#include <cstdint>
#include "app/engine/HeaterEngineState.h"
#include "core/util/control/PidControl.h"
#include "core/util/fsm/FiniteStateMachine.h"

namespace Garbox {

class HeaterEngine {
public:

    using State = HeaterEngineState;

    struct Input {

        // temperature control
        float targetTemperature = 0.0f; // Celcius 

        // measured temperature
        bool  measuredTemperatureValid = false;
        float measuredTemperature = 0.0f; // Celcius

        // measured relative humidity
        bool  measuredHumidityValid = false;
        float measuredHumidity = 0.0f;

        // fan stall detection
        bool  fanStalled = false;
        float fanMeasuredRpm = 0.0f;
    };

    struct Output {

        // new fan state 
        bool  fanEnabled;
        float fanTargetSpeed;

        // new heatpad
        bool  heaterEnabled;
        float heaterPwmDuty;
        float heaterPwmPeriodMicros;
    };

    HeaterEngine();

    void reset();
    void step();

    void setRegulationEnabled(bool enabled);

    Input& getInput() { return mInput; };
    const Output& getOutput() const { return mOutput; };
    State getState() const { return mFsm.getState(); }
    bool isRegulationEnabled() const { return mRegulationEnabled; }

private:

    static constexpr float CooldownStateThresholdTemperature = 30.0f; 

    // force off if temperature too far above target temperature
    static constexpr float OverTemperatureDelta = 2.0f; 

    // temperature must be below over-temperature threshold for at least 10s before going back to regulating state
    static constexpr uint32_t OverTemperatureExitTimeMicros = 10_s; 

    // control
    bool mRegulationEnabled = false;
    PidControl mPid;

    // input and output
    Input mInput {}; // zero initialized
    Output mOutput {}; // zero initialized

    // input tracking
    float mLastTargetTemperature = 0.0f;
    bool mTargetTemperatureChanged = false;

    // FSM
    FiniteStateMachine<State, State::Count> mFsm;
    void handleResetState();
    void handleReadyState();
    void handleRegulatingState();
    void handleInvalidInputState();
    void handleOverTemperatureState();
    void handleCooldownState();
    void handleFsmStateChanged(State oldState, State newState);

    // internal methods
    bool isInOverTemperatureZone() const;
    bool isInputInvalid() const;

};

} // namespace