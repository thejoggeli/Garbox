#pragma once

#include <cstdint>

namespace Garbox {

class I2c;

class Sht31 {
public:

    enum class PeriodicMode : uint8_t {
        Hz05, // 0.5 Hz
        Hz1,  // 1   Hz
        Hz2,  // 2   Hz
        Hz4,  // 4   Hz
        Hz10  // 10  Hz
    };

    struct Config {
        uint8_t address = 0x44;
        PeriodicMode mode = PeriodicMode::Hz1;
    };

    Sht31(I2c& i2c);

    void init(const Config& config);
    bool tryFetch();

    bool start(); // starts reading periodically
    bool stop(); // stop reading periodically 
    void reset();

    bool isInitialized() const;
    bool isStarted() const;

    float getTemperatureCelcius() const;
    float getHumidityRelative() const;
    bool hasValidReading() const;

    // disallow copy and move
    Sht31(const Sht31&) = delete;
    Sht31& operator=(const Sht31&) = delete;
    Sht31(Sht31&&) = delete;
    Sht31& operator=(Sht31&&) = delete;

private:
    static constexpr uint32_t FetchIntervalMillis = 1000;
    static constexpr uint16_t StopPeriodicCmd = 0x3093;

    I2c& mI2c;
    uint8_t mAddress = 0x44;
    PeriodicMode mMode = PeriodicMode::Hz05;

    bool mInitialized = false;
    bool mHasReading = false;
    bool mStarted = false;

    uint32_t mLastFetchTimeMillis = 0;

    float mLastTemperatureC = 0.0f;
    float mLastHumidityRh = 0.0f;

    bool startPeriodicMode();
    bool fetchMeasurement();
    bool checkCrc(const uint8_t* data, uint32_t len, uint8_t crc) const;

    uint16_t getPeriodicCommand() const;
};

} // namespace Garbox
