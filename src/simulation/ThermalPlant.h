#pragma once

namespace Garbox {

/**
 * Small deterministic plant for application testing.
 *
 * Parameters are intentionally illustrative, not calibrated to the physical
 * enclosure. The model is useful for closed-loop transitions and invariants;
 * it must not be used to predict real temperatures or safety margins.
 */
class ThermalPlant {
public:
    struct Config {
        float ambientTemperatureCelcius = 20.0f;
        float initialTemperatureCelcius = 20.0f;
        float humidityRelative = 60.0f;
        float heaterGainCelciusPerSecond = 0.05f;
        float passiveLossPerSecond = 0.0015f;
        float fanLossPerSecond = 0.006f;
        float fanResponsePerSecond = 2.5f;
        float fanMaximumRpm = 2400.0f;
    };

    ThermalPlant();
    explicit ThermalPlant(const Config& config);

    void setHeater(bool enabled, float dutyCycle);
    void setFan(bool enabled, float speed);
    void setFanStalled(bool stalled);
    void setSensorAvailable(bool available);
    void setTemperatureCelcius(float temperatureCelcius);
    void step(float deltaSeconds);

    bool sensorAvailable() const;
    bool fanStalled() const;
    float temperatureCelcius() const;
    float humidityRelative() const;
    float fanRpm() const;

private:
    Config mConfig;
    bool mHeaterEnabled = false;
    float mHeaterDutyCycle = 0.0f;
    bool mFanEnabled = false;
    float mFanSpeed = 0.0f;
    bool mFanStalled = false;
    bool mSensorAvailable = true;
    float mTemperatureCelcius = 20.0f;
    float mFanRpm = 0.0f;
};

} // namespace Garbox
