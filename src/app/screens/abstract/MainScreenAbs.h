#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/guis/MainScreenGui.h"
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class MainScreenAbs : public ScreenAbs {
public:

    // constructor 
    MainScreenAbs();

    // init override from component
    void init(ComponentHostIfc& host) final;
    void updateScreen() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

    // event handlers (to be implmeneted by user)
    virtual void onDisplayCommand(const DisplayCommandEvent& event) = 0;
    virtual void onFanCommand(const FanCommandEvent& event) = 0;
    virtual void onHeatpadCommand(const HeatpadCommandEvent& event) = 0;

protected:

    // model field callbacks to update specific parts of the screen (to be implmeneted by user)
    virtual void onApplyFanStatus() = 0;
    virtual void onApplyFanTargetSpeed() = 0;
    virtual void onApplyHeatpadStatus() = 0;
    virtual void onApplyHeatpadMeasure() = 0;
    virtual void onApplyDisplayBrightness() = 0;
    virtual void onApplySensorStatus() = 0;
    virtual void onApplyMeasuredTemperature() = 0;
    virtual void onApplyMeasuredHumidity() = 0;
    virtual void onApplyTargetTemperature() = 0;
    virtual void onApplyEngineState() = 0;

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

/*******************/
/*** GUI Section ***/
/*******************/

private:

    MainScreenGui mGui;

protected:

    MainScreenGui::Objects& gui(); 
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

/*********************/
/*** Model Section ***/
/*********************/

public: 

    class Model { 
    public: 

        // dirty-flag indices (one per group) 
        enum class Index : uint8_t { 
            FanStatus = 0,
            FanTargetSpeed,
            HeatpadStatus,
            HeatpadMeasure,
            DisplayBrightness,
            SensorStatus,
            MeasuredTemperature,
            MeasuredHumidity,
            TargetTemperature,
            EngineState,
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
        float getFanMeasuredRpm() const;
        float getFanTargetSpeed() const;
        HeatpadState getHeatpadState() const;
        float getHeatpadCurrentDuty() const;
        float getHeatpadMeasuredVoltage() const;
        float getHeatpadMeasuredCurrent() const;
        float getDisplayBrightness() const;
        bool getShtDriverEnabled() const;
        bool getShtPowerEnabled() const;
        bool getShtResetting() const;
        bool getShtHasSample() const;
        float getMeasuredTemperature() const;
        float getMeasuredHumidity() const;
        float getTargetTemperature() const;
        HeaterEngineState getEngineState() const;

        // setters 
        void setFanState(FanState value);
        void setFanMeasuredRpm(float value);
        void setFanTargetSpeed(float value);
        void setHeatpadState(HeatpadState value);
        void setHeatpadCurrentDuty(float value);
        void setHeatpadMeasuredVoltage(float value);
        void setHeatpadMeasuredCurrent(float value);
        void setDisplayBrightness(float value);
        void setShtDriverEnabled(bool value);
        void setShtPowerEnabled(bool value);
        void setShtResetting(bool value);
        void setShtHasSample(bool value);
        void setMeasuredTemperature(float value);
        void setMeasuredHumidity(float value);
        void setTargetTemperature(float value);
        void setEngineState(HeaterEngineState value);

    private: 

        MainScreenAbs& mScreen; 

        // all model fields
        FanState mFanState;
        float mFanMeasuredRpm;
        float mFanTargetSpeed;
        HeatpadState mHeatpadState;
        float mHeatpadCurrentDuty;
        float mHeatpadMeasuredVoltage;
        float mHeatpadMeasuredCurrent;
        float mDisplayBrightness;
        bool mShtDriverEnabled;
        bool mShtPowerEnabled;
        bool mShtResetting;
        bool mShtHasSample;
        float mMeasuredTemperature;
        float mMeasuredHumidity;
        float mTargetTemperature;
        HeaterEngineState mEngineState;

    };

private:

    // model instance
    Model mModel;

public:

    Model& model();
    const Model& model() const;

protected: 

    // dispatcher (calls apply methods)
    DirtyDispatcher mDirtyDispatcher;
    uint32_t mDispatchedCount = 0;
    uint32_t getDispatchedCount() const { return mDispatchedCount; }

    // method to mark an model field index dirty (for manually updated values)
    void markDirty(Model::Index index);
    bool isMarkedDirty(Model::Index index) const;

private: 

    // model field apply trampolines
    static void applyFanStatusTrampoline(void* context);
    static void applyFanTargetSpeedTrampoline(void* context);
    static void applyHeatpadStatusTrampoline(void* context);
    static void applyHeatpadMeasureTrampoline(void* context);
    static void applyDisplayBrightnessTrampoline(void* context);
    static void applySensorStatusTrampoline(void* context);
    static void applyMeasuredTemperatureTrampoline(void* context);
    static void applyMeasuredHumidityTrampoline(void* context);
    static void applyTargetTemperatureTrampoline(void* context);
    static void applyEngineStateTrampoline(void* context);

};

} // namespace Garbox