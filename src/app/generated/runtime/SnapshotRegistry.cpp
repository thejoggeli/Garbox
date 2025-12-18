// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "SnapshotRegistry.h"
#include "core/assert/Assert.h"

namespace Garbox {

SnapshotRegistry::SnapshotRegistry(){
    // nothing to do
}

SnapshotRegistry::~SnapshotRegistry(){
    // nothing to do
}

SnapshotRegistry& SnapshotRegistry::instance(){
    static SnapshotRegistry instance;
    return instance;    
}

void SnapshotRegistry::UpdateFermentationStatus(const FermentationStatusPayload& payload){
    instance().mFermentationStatusPayload = payload;
}

const FermentationStatusPayload& SnapshotRegistry::GetFermentationStatus(){
    return instance().mFermentationStatusPayload;
}

void SnapshotRegistry::UpdateDisplayStatus(const DisplayStatusPayload& payload){
    instance().mDisplayStatusPayload = payload;
}

const DisplayStatusPayload& SnapshotRegistry::GetDisplayStatus(){
    return instance().mDisplayStatusPayload;
}

void SnapshotRegistry::UpdateFanStatus(const FanStatusPayload& payload){
    instance().mFanStatusPayload = payload;
}

const FanStatusPayload& SnapshotRegistry::GetFanStatus(){
    return instance().mFanStatusPayload;
}

void SnapshotRegistry::UpdateFanSample(const FanSamplePayload& payload){
    instance().mFanSamplePayload = payload;
}

const FanSamplePayload& SnapshotRegistry::GetFanSample(){
    return instance().mFanSamplePayload;
}

void SnapshotRegistry::UpdateHeatpadStatus(const HeatpadStatusPayload& payload){
    instance().mHeatpadStatusPayload = payload;
}

const HeatpadStatusPayload& SnapshotRegistry::GetHeatpadStatus(){
    return instance().mHeatpadStatusPayload;
}

void SnapshotRegistry::UpdateHeatpadSample(const HeatpadSamplePayload& payload){
    instance().mHeatpadSamplePayload = payload;
}

const HeatpadSamplePayload& SnapshotRegistry::GetHeatpadSample(){
    return instance().mHeatpadSamplePayload;
}

void SnapshotRegistry::UpdateTemperatureStatus(const TemperatureStatusPayload& payload){
    instance().mTemperatureStatusPayload = payload;
}

const TemperatureStatusPayload& SnapshotRegistry::GetTemperatureStatus(){
    return instance().mTemperatureStatusPayload;
}

void SnapshotRegistry::UpdateTemperatureSample(const TemperatureSamplePayload& payload){
    instance().mTemperatureSamplePayload = payload;
}

const TemperatureSamplePayload& SnapshotRegistry::GetTemperatureSample(){
    return instance().mTemperatureSamplePayload;
}

void SnapshotRegistry::UpdateActiveBehaviourChanged(const ActiveBehaviourChangedPayload& payload){
    instance().mActiveBehaviourChangedPayload = payload;
}

const ActiveBehaviourChangedPayload& SnapshotRegistry::GetActiveBehaviourChanged(){
    return instance().mActiveBehaviourChangedPayload;
}

void SnapshotRegistry::UpdateActiveScreenChanged(const ActiveScreenChangedPayload& payload){
    instance().mActiveScreenChangedPayload = payload;
}

const ActiveScreenChangedPayload& SnapshotRegistry::GetActiveScreenChanged(){
    return instance().mActiveScreenChangedPayload;
}

} // namespace