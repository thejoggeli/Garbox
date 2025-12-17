#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {
public:

    // constructor 
    DebugScreenAbs();

    // init override from component
    void init(ComponentHostIfc& host) final;
    void updateScreen() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

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
    virtual void onApplyTime() = 0;
    virtual void onApplyAppInfo() = 0;
    virtual void onApplyFermentationStatus() = 0;
    virtual void onApplyHeapBlocks() = 0;
    virtual void onApplyHeapBytes() = 0;
    virtual void onApplyHeapMinimum() = 0;

    // make typed events 
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

/********************/
/*** LVGL Section ***/
/********************/

protected:

    LvObject mRoot;
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
            FanState = 0,
            FanMeasuredRpm,
            HeatpadState,
            HeatpadDuty,
            BoxPosition,
            HeatpadSense,
            DisplayStatus,
            TemperatureState,
            TemperatureSample,
            Time,
            AppInfo,
            FermentationStatus,
            HeapBlocks,
            HeapBytes,
            HeapMinimum,
            Count 
        }; 

        Model(DebugScreenAbs& screen); 

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
        uint32_t getTimeSeconds() const;
        uint32_t getEventCount() const;
        BehaviourId getBehaviour() const;
        FermentationState getEngineState() const;
        float getEngineTargetTemperature() const;
        uint32_t getHeapAllocatedBlocks() const;
        uint32_t getHeapFreeBlocks() const;
        uint32_t getHeapLargestFreeBlock() const;
        uint32_t getHeapAllocatedBytes() const;
        uint32_t getHeapMinimumFreeBytes() const;
        uint32_t getHeapTotalFreeBytes() const;
        uint32_t getHeapMinimumTime() const;

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
        void setTimeSeconds(uint32_t value);
        void setEventCount(uint32_t value);
        void setBehaviour(BehaviourId value);
        void setEngineState(FermentationState value);
        void setEngineTargetTemperature(float value);
        void setHeapAllocatedBlocks(uint32_t value);
        void setHeapFreeBlocks(uint32_t value);
        void setHeapLargestFreeBlock(uint32_t value);
        void setHeapAllocatedBytes(uint32_t value);
        void setHeapMinimumFreeBytes(uint32_t value);
        void setHeapTotalFreeBytes(uint32_t value);
        void setHeapMinimumTime(uint32_t value);

    private: 

        DebugScreenAbs& mScreen; 

        // all model fields
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
        uint32_t mTimeSeconds;
        uint32_t mEventCount;
        BehaviourId mBehaviour;
        FermentationState mEngineState;
        float mEngineTargetTemperature;
        uint32_t mHeapAllocatedBlocks;
        uint32_t mHeapFreeBlocks;
        uint32_t mHeapLargestFreeBlock;
        uint32_t mHeapAllocatedBytes;
        uint32_t mHeapMinimumFreeBytes;
        uint32_t mHeapTotalFreeBytes;
        uint32_t mHeapMinimumTime;

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

    void setSnapshotFanStatus();
    void setSnapshotFanSample();
    void setSnapshotHeatpadStatus();
    void setSnapshotHeatpadSample();
    void setSnapshotDisplayStatus();
    void setSnapshotTemperatureStatus();
    void setSnapshotTemperatureSample();
    void setSnapshotActiveBehaviourChanged();
    void setSnapshotFermentationStatus();

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
    static void applyTimeTrampoline(void* context);
    static void applyAppInfoTrampoline(void* context);
    static void applyFermentationStatusTrampoline(void* context);
    static void applyHeapBlocksTrampoline(void* context);
    static void applyHeapBytesTrampoline(void* context);
    static void applyHeapMinimumTrampoline(void* context);

};

} // namespace Garbox