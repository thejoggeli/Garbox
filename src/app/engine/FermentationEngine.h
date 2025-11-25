#pragma once

#include <cstdint>

namespace Garbox {

class FermentationEngine {
public:

    struct Inputs {

        // temperature sensor
        float measuredTemperatureDegC;

        // fan input
        bool fanEnabled;
        bool fanStalled;
        float fanTargetSpeed;
        float fanMeasuredRpm;

        // heatpad
        bool heatpadEnabled;
        float heatpadPwmDuty;
        uint32_t heatpadPwmPeriodMicros;

        // temperature and humidity
        bool temperatureEnabled;
        bool temperatureError;
        float temperatureCelcius;
        float humidityRelative;
    };

    struct Outputs {

        // fan 
        bool fanEnabled;
        float fanTargetSpeed;

        // heatpad
        bool heatpadEnabled;
        float heatpadPwmDuty;
        float heatpadPwmPeriodSeconds;
    };

    FermentationEngine();

    void init();
    void reset();
    void step();

    Inputs& getInputs();
    const Outputs& getOutputs() const;

private:

    bool mInitialized = false;
    Inputs mInputs {}; // zero initialized
    Outputs mOutputs {}; // zero initialized

};

} // namespace