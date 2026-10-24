#include "simulation/ThermalPlant.h"

#include <algorithm>
#include <cmath>

namespace Garbox {

ThermalPlant::ThermalPlant(): ThermalPlant(Config {}){}

ThermalPlant::ThermalPlant(const Config& config):
    mConfig(config),
    mTemperatureCelcius(config.initialTemperatureCelcius){}

void ThermalPlant::setHeater(bool enabled, float dutyCycle){
    mHeaterEnabled = enabled;
    mHeaterDutyCycle = std::clamp(dutyCycle, 0.0f, 1.0f);
}

void ThermalPlant::setFan(bool enabled, float speed){
    mFanEnabled = enabled;
    mFanSpeed = std::clamp(speed, 0.0f, 1.0f);
}

void ThermalPlant::setFanStalled(bool stalled){
    mFanStalled = stalled;
}

void ThermalPlant::setSensorAvailable(bool available){
    mSensorAvailable = available;
}

void ThermalPlant::setTemperatureCelcius(float temperatureCelcius){
    mTemperatureCelcius = temperatureCelcius;
}

void ThermalPlant::step(float deltaSeconds){
    if(!std::isfinite(deltaSeconds) || deltaSeconds <= 0.0f){
        return;
    }

    const float fanTargetRpm = (mFanEnabled && !mFanStalled)
        ? mConfig.fanMaximumRpm * mFanSpeed
        : 0.0f;
    const float fanBlend = std::clamp(mConfig.fanResponsePerSecond * deltaSeconds, 0.0f, 1.0f);
    mFanRpm += (fanTargetRpm - mFanRpm) * fanBlend;

    const float aboveAmbient = std::max(
        mTemperatureCelcius - mConfig.ambientTemperatureCelcius,
        0.0f
    );
    const float heatRate = mHeaterEnabled
        ? mConfig.heaterGainCelciusPerSecond * mHeaterDutyCycle
        : 0.0f;
    const float passiveLoss = mConfig.passiveLossPerSecond * aboveAmbient;
    const float fanLoss = (mFanEnabled && !mFanStalled)
        ? mConfig.fanLossPerSecond * mFanSpeed * aboveAmbient
        : 0.0f;

    mTemperatureCelcius += (heatRate - passiveLoss - fanLoss) * deltaSeconds;
}

bool ThermalPlant::sensorAvailable() const {
    return mSensorAvailable;
}

bool ThermalPlant::fanStalled() const {
    return mFanStalled;
}

float ThermalPlant::temperatureCelcius() const {
    return mTemperatureCelcius;
}

float ThermalPlant::humidityRelative() const {
    return mConfig.humidityRelative;
}

float ThermalPlant::fanRpm() const {
    return mFanRpm;
}

} // namespace Garbox
