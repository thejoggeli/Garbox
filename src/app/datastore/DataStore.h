#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/datastore/DataStoreAlias.h"
#include "core/application/datastore/DataStoreAbs.h"

namespace Garbox {

struct DataStore : public DataStoreAbs {
public:

    static void Init();

    static const StoredTempsenseState& GetTempsenseState();
    static bool ResetTempsenseState();
    static bool UpdateTempsenseState(TempsenseState value);

    static const StoredBool& GetTempsenseDriverEnabled();
    static bool ResetTempsenseDriverEnabled();
    static bool UpdateTempsenseDriverEnabled(bool value);

    static const StoredBool& GetTempsensePowerEnabled();
    static bool ResetTempsensePowerEnabled();
    static bool UpdateTempsensePowerEnabled(bool value);

    static const StoredBool& GetTempsenseResetting();
    static bool ResetTempsenseResetting();
    static bool UpdateTempsenseResetting(bool value);

    static const StoredBool& GetTempsenseHasFirstSample();
    static bool ResetTempsenseHasFirstSample();
    static bool UpdateTempsenseHasFirstSample(bool value);

    static const StoredFloat& GetMeasuredTemperature();
    static bool ResetMeasuredTemperature();
    static bool UpdateMeasuredTemperature(float value);

    static const StoredFloat& GetMeasuredHumidity();
    static bool ResetMeasuredHumidity();
    static bool UpdateMeasuredHumidity(float value);

    static const StoredFloat& GetDisplayBrightness();
    static bool ResetDisplayBrightness();
    static bool UpdateDisplayBrightness(float value);

    static const StoredUint32& GetFramesSkipped();
    static bool ResetFramesSkipped();
    static bool UpdateFramesSkipped(uint32_t value);

    static const StoredHeaterEngineState& GetHeaterEngineState();
    static bool ResetHeaterEngineState();
    static bool UpdateHeaterEngineState(HeaterEngineState value);

    static const StoredFloat& GetTargetTemperature();
    static bool ResetTargetTemperature();
    static bool UpdateTargetTemperature(float value);

    static const StoredFanState& GetFanState();
    static bool ResetFanState();
    static bool UpdateFanState(FanState value);

    static const StoredFloat& GetFanTargetSpeed();
    static bool ResetFanTargetSpeed();
    static bool UpdateFanTargetSpeed(float value);

    static const StoredBool& GetFanRpmControl();
    static bool ResetFanRpmControl();
    static bool UpdateFanRpmControl(bool value);

    static const StoredFloat& GetFanMeasuredRpm();
    static bool ResetFanMeasuredRpm();
    static bool UpdateFanMeasuredRpm(float value);

    static const StoredHeatpadState& GetHeatpadState();
    static bool ResetHeatpadState();
    static bool UpdateHeatpadState(HeatpadState value);

    static const StoredFloat& GetHeatpadDuty();
    static bool ResetHeatpadDuty();
    static bool UpdateHeatpadDuty(float value);

    static const StoredUint32& GetHeatpadPeriod();
    static bool ResetHeatpadPeriod();
    static bool UpdateHeatpadPeriod(uint32_t value);

    static const StoredFloat& GetHeatpadNextDuty();
    static bool ResetHeatpadNextDuty();
    static bool UpdateHeatpadNextDuty(float value);

    static const StoredUint32& GetHeatpadNextPeriod();
    static bool ResetHeatpadNextPeriod();
    static bool UpdateHeatpadNextPeriod(uint32_t value);

    static const StoredFloat& GetHeatpadMeasuredVoltage();
    static bool ResetHeatpadMeasuredVoltage();
    static bool UpdateHeatpadMeasuredVoltage(float value);

    static const StoredFloat& GetHeatpadMeasuredCurrent();
    static bool ResetHeatpadMeasuredCurrent();
    static bool UpdateHeatpadMeasuredCurrent(float value);

    static const StoredUint32& GetHeatpadPwmProgress();
    static bool ResetHeatpadPwmProgress();
    static bool UpdateHeatpadPwmProgress(uint32_t value);

private:

    DataStore();
    ~DataStore();

    static DataStore& instance();

    StoredTempsenseState mTempsenseState;
    StoredBool mTempsenseDriverEnabled;
    StoredBool mTempsensePowerEnabled;
    StoredBool mTempsenseResetting;
    StoredBool mTempsenseHasFirstSample;
    StoredFloat mMeasuredTemperature;
    StoredFloat mMeasuredHumidity;
    StoredFloat mDisplayBrightness;
    StoredUint32 mFramesSkipped;
    StoredHeaterEngineState mHeaterEngineState;
    StoredFloat mTargetTemperature;
    StoredFanState mFanState;
    StoredFloat mFanTargetSpeed;
    StoredBool mFanRpmControl;
    StoredFloat mFanMeasuredRpm;
    StoredHeatpadState mHeatpadState;
    StoredFloat mHeatpadDuty;
    StoredUint32 mHeatpadPeriod;
    StoredFloat mHeatpadNextDuty;
    StoredUint32 mHeatpadNextPeriod;
    StoredFloat mHeatpadMeasuredVoltage;
    StoredFloat mHeatpadMeasuredCurrent;
    StoredUint32 mHeatpadPwmProgress;

};

} // namespace