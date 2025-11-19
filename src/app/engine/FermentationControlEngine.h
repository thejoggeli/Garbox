#pragma once

namespace Garbox {

class FermentationControlEngine {
public:

    struct Inputs {

        // temperature sensor
        float measuredTemperatureDegC;

        // fan input
        bool fanEnabled;
        float fanTargetSpeed;
        float fanMeasuredRpm;

        // heatpad
        bool heatpadEnabled;
        float heatpadPwmDuty;
        float heatpadPwmPeriodSeconds;
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

    FermentationControlEngine();

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