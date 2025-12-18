#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "shared/types/EventAlias.h"

namespace Garbox {

class SnapshotRegistry {
public:

    static void UpdateFermentationStatus(const FermentationStatusPayload& payload);
    static void UpdateDisplayStatus(const DisplayStatusPayload& payload);
    static void UpdateFanStatus(const FanStatusPayload& payload);
    static void UpdateFanSample(const FanSamplePayload& payload);
    static void UpdateHeatpadStatus(const HeatpadStatusPayload& payload);
    static void UpdateHeatpadSample(const HeatpadSamplePayload& payload);
    static void UpdateTemperatureStatus(const TemperatureStatusPayload& payload);
    static void UpdateTemperatureSample(const TemperatureSamplePayload& payload);
    static void UpdateActiveBehaviourChanged(const ActiveBehaviourChangedPayload& payload);
    static void UpdateActiveScreenChanged(const ActiveScreenChangedPayload& payload);

    static const FermentationStatusPayload& GetFermentationStatus();
    static const DisplayStatusPayload& GetDisplayStatus();
    static const FanStatusPayload& GetFanStatus();
    static const FanSamplePayload& GetFanSample();
    static const HeatpadStatusPayload& GetHeatpadStatus();
    static const HeatpadSamplePayload& GetHeatpadSample();
    static const TemperatureStatusPayload& GetTemperatureStatus();
    static const TemperatureSamplePayload& GetTemperatureSample();
    static const ActiveBehaviourChangedPayload& GetActiveBehaviourChanged();
    static const ActiveScreenChangedPayload& GetActiveScreenChanged();

private:

    SnapshotRegistry();
    ~SnapshotRegistry();
    SnapshotRegistry(const SnapshotRegistry&) = delete;
    SnapshotRegistry& operator=(const SnapshotRegistry&) = delete;
    SnapshotRegistry(SnapshotRegistry&&) = delete;
    SnapshotRegistry& operator=(SnapshotRegistry&&) = delete;

    static SnapshotRegistry& instance();

    FermentationStatusPayload mFermentationStatusPayload;
    DisplayStatusPayload mDisplayStatusPayload;
    FanStatusPayload mFanStatusPayload;
    FanSamplePayload mFanSamplePayload;
    HeatpadStatusPayload mHeatpadStatusPayload;
    HeatpadSamplePayload mHeatpadSamplePayload;
    TemperatureStatusPayload mTemperatureStatusPayload;
    TemperatureSamplePayload mTemperatureSamplePayload;
    ActiveBehaviourChangedPayload mActiveBehaviourChangedPayload;
    ActiveScreenChangedPayload mActiveScreenChangedPayload;

};

} // namespace