#include "Sht31.h"

#include "core/assert/Assert.h"
#include "core/hardware/i2c/I2c.h"
#include "core/time/Time.h"
#include "core/util/math/Crc.h"
#include "core/util/helpers/ByteUtils.h"

namespace Garbox {

Sht31::Sht31(I2c& i2c) : mI2c(i2c) {
    // nothing to do
}

void Sht31::init(const Config& config){
    AssertExit(!mInitialized, "Sht31", "already initialized");
    mAddress = config.address;
    mMode = config.mode;
    mInitialized = true;
}

bool Sht31::start(){
    if(!mInitialized){
        TriggerDebug("Sht31", "not initialized");
        return false;
    }

    const bool ok = startPeriodicMode();
    if(!ok){
        TriggerDebug("Sht31", "failed to start periodic mode");
        return false;
    }

    mLastFetchTimeMillis = Time::GetMillis();
    mStarted = true;

    return true;
}

bool Sht31::stop(){
    if(!mInitialized){
        TriggerDebug("Sht31", "not initialized");
        return false;
    }

    uint8_t cmd[2];
    cmd[0] = ByteUtils::GetByte1(StopPeriodicCmd);
    cmd[1] = ByteUtils::GetByte0(StopPeriodicCmd);

    const bool ok = mI2c.write(mAddress, cmd, 2);
    if(!ok){
        TriggerDebug("Sht31", "failed to stop periodic mode");
        return false;
    }

    mStarted = false;
    mHasReading = false;

    return true;
}

void Sht31::reset(){
    if(!mInitialized){
        TriggerDebug("Sht31", "not initialized");
        return;
    }
    mStarted = false;
    mHasReading = false;
}

uint16_t Sht31::getPeriodicCommand() const {
    switch(mMode){
    case PeriodicMode::Hz05: return 0x2032;
    case PeriodicMode::Hz1:  return 0x2130;
    case PeriodicMode::Hz2:  return 0x2236;
    case PeriodicMode::Hz4:  return 0x2334;
    case PeriodicMode::Hz10: return 0x2737;
    default:
        return 0x2130;
    }
}

bool Sht31::startPeriodicMode(){
    const uint16_t cmdValue = getPeriodicCommand();

    uint8_t cmd[2];
    cmd[0] = ByteUtils::GetByte1(cmdValue);
    cmd[1] = ByteUtils::GetByte0(cmdValue);

    const bool ok = mI2c.write(mAddress, cmd, 2);

    return ok;
}

bool Sht31::tryFetch(){
    if(!mInitialized){
        TriggerDebug("Sht31", "not initialized");
        return false;
    }

    if(!mStarted){
        return false;
    }

    const uint32_t nowMillis = Time::GetMillis();
    const bool shouldFetch = ((nowMillis - mLastFetchTimeMillis) >= FetchIntervalMillis);

    if(!shouldFetch){
        return false;
    }

    mLastFetchTimeMillis = nowMillis;

    const bool ok = fetchMeasurement();
    if(!ok){
        return false;
    }

    mHasReading = true;
    return true;
}

bool Sht31::fetchMeasurement(){
    uint8_t buf[6] = {0};

    const uint32_t lengthBytes = 6;
    const bool ok = mI2c.read(mAddress, buf, lengthBytes);

    if(!ok){
        return false;
    }

    constexpr uint32_t crcLength = 2;
    const bool tempOk = checkCrc(&buf[0], crcLength, buf[2]);
    const bool humOk = checkCrc(&buf[3], crcLength, buf[5]);

    if(!tempOk || !humOk){
        TriggerDebug("Sht31", "crc error");
        return false;
    }

    const uint16_t rawTemp = ByteUtils::MakeUint16(buf[1], buf[0]);
    const float tempC = -45.0f + (175.0f * static_cast<float>(rawTemp) / 65535.0f);

    const uint16_t rawHum = ByteUtils::MakeUint16(buf[4], buf[3]);
    const float rh = 100.0f * static_cast<float>(rawHum) / 65535.0f;

    mLastTemperatureC = tempC;
    mLastHumidityRh = rh;

    return true;
}

bool Sht31::checkCrc(const uint8_t* data, uint32_t len, uint8_t crc) const {
    constexpr uint8_t initialValue = 0xFF;
    const uint8_t calc = Crc::Crc8(data, len, initialValue);
    return (calc == crc);
}

bool Sht31::isInitialized() const {
    return mInitialized;
}

bool Sht31::isStarted() const {
    return mStarted;
}

float Sht31::getTemperatureCelcius() const {
    return mLastTemperatureC;
}

float Sht31::getHumidityRelative() const {
    return mLastHumidityRh;
}

bool Sht31::hasValidReading() const {
    return mHasReading;
}

} // namespace Garbox
