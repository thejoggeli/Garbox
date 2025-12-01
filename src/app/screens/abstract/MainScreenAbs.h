#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class MainScreenAbs : public ScreenAbs {
protected:

    // model field callbacks to update specific parts of the screen (to be implmeneted by user)
    virtual void onApplyFanState() = 0;
    virtual void onApplyFanMeasuredRpm() = 0;
    virtual void onApplyHeatpadState() = 0;
    virtual void onApplyHeatpadDuty() = 0;
    virtual void onApplyBoxPosition() = 0;
    virtual void onApplyHeatpadSense() = 0;
    virtual void onApplyDisplayStatus() = 0;
    virtual void onApplyTemperatureState() = 0;
    virtual void onApplyTemperatureSample() = 0;
    virtual void onApplyAppInfo() = 0;
    virtual void onApplyFermentationStatus() = 0;
    virtual void onApplyHeapSpace() = 0;

public:

    MainScreenAbs();

    class Model { 
    public: 

        // dirty-flag indices (one per group) 
        enum class Index : uint8_t { 
            FanState = 0,
            FanMeasuredRpm,
            HeatpadState,
            HeatpadDuty,
            BoxPosition,
            HeatpadSense,
            DisplayStatus,
            TemperatureState,
            TemperatureSample,
            AppInfo,
            FermentationStatus,
            HeapSpace,
            Count 
        }; 

        Model(MainScreenAbs& screen); 

        // disallow copy and move 
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;
        Model(Model&&) = delete;
        Model& operator=(Model&&) = delete;

        // getters
        FanState getFanState() const;
        float getFanTargetSpeed() const;
        float getFanMeasuredRpm() const;
        HeatpadState getHeatpadState() const;
        float getHeatpadCurrentDuty() const;
        uint32_t getHeatpadCurrentPeriod() const;
        float getHeatpadNextDuty() const;
        uint32_t getHeatpadNextPeriod() const;
        float getHeatpadPwmProgress() const;
        float getHeatpadMeasuredVoltage() const;
        float getHeatpadMeasuredCurrent() const;
        float getDisplayBrightness() const;
        uint32_t getDisplaySkipped() const;
        bool getShtDriverEnabled() const;
        bool getShtPowerEnabled() const;
        bool getShtResetting() const;
        float getSensorTemperatureCelcius() const;
        float getSensorHumidityRelative() const;
        uint32_t getEventCount() const;
        BehaviourId getBehaviour() const;
        HeaterEngineState getEngineState() const;
        float getEngineTargetTemperature() const;
        float getEngineMeasuredTemperature() const;
        float getEngineMeasuredHumidity() const;
        uint32_t getHeapSpace() const;

        // setters 
        void setFanState(FanState value);
        void setFanTargetSpeed(float value);
        void setFanMeasuredRpm(float value);
        void setHeatpadState(HeatpadState value);
        void setHeatpadCurrentDuty(float value);
        void setHeatpadCurrentPeriod(uint32_t value);
        void setHeatpadNextDuty(float value);
        void setHeatpadNextPeriod(uint32_t value);
        void setHeatpadPwmProgress(float value);
        void setHeatpadMeasuredVoltage(float value);
        void setHeatpadMeasuredCurrent(float value);
        void setDisplayBrightness(float value);
        void setDisplaySkipped(uint32_t value);
        void setShtDriverEnabled(bool value);
        void setShtPowerEnabled(bool value);
        void setShtResetting(bool value);
        void setSensorTemperatureCelcius(float value);
        void setSensorHumidityRelative(float value);
        void setEventCount(uint32_t value);
        void setBehaviour(BehaviourId value);
        void setEngineState(HeaterEngineState value);
        void setEngineTargetTemperature(float value);
        void setEngineMeasuredTemperature(float value);
        void setEngineMeasuredHumidity(float value);
        void setHeapSpace(uint32_t value);

    private: 

        MainScreenAbs& mScreen; 

        FanState mFanState;
        float mFanTargetSpeed;
        float mFanMeasuredRpm;
        HeatpadState mHeatpadState;
        float mHeatpadCurrentDuty;
        uint32_t mHeatpadCurrentPeriod;
        float mHeatpadNextDuty;
        uint32_t mHeatpadNextPeriod;
        float mHeatpadPwmProgress;
        float mHeatpadMeasuredVoltage;
        float mHeatpadMeasuredCurrent;
        float mDisplayBrightness;
        uint32_t mDisplaySkipped;
        bool mShtDriverEnabled;
        bool mShtPowerEnabled;
        bool mShtResetting;
        float mSensorTemperatureCelcius;
        float mSensorHumidityRelative;
        uint32_t mEventCount;
        BehaviourId mBehaviour;
        HeaterEngineState mEngineState;
        float mEngineTargetTemperature;
        float mEngineMeasuredTemperature;
        float mEngineMeasuredHumidity;
        uint32_t mHeapSpace;

    };

    Model& getModel();
    const Model& getModel() const;

protected:

    Model mModel;

    // method to mark an model field index dirty (for manually updated values)
    void markDirty(Model::Index index);
    bool isMarkedDirty(Model::Index index) const;

    // make typed events 
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    void onInitScreen() final;

    // model field apply trampolines
    static void applyFanStateTrampoline(void* context);
    static void applyFanMeasuredRpmTrampoline(void* context);
    static void applyHeatpadStateTrampoline(void* context);
    static void applyHeatpadDutyTrampoline(void* context);
    static void applyBoxPositionTrampoline(void* context);
    static void applyHeatpadSenseTrampoline(void* context);
    static void applyDisplayStatusTrampoline(void* context);
    static void applyTemperatureStateTrampoline(void* context);
    static void applyTemperatureSampleTrampoline(void* context);
    static void applyAppInfoTrampoline(void* context);
    static void applyFermentationStatusTrampoline(void* context);
    static void applyHeapSpaceTrampoline(void* context);

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

    // hide dangerous raw access
    using ScreenAbs::registerUpdateHandler;
    using ScreenAbs::markDirtyRaw;
    using ScreenAbs::isMarkedDirtyRaw;

};

} // namespace Garbox