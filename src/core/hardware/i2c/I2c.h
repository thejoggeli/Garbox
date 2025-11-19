#pragma once

#include <cstdint>
#include "driver/i2c.h"

namespace Garbox {

class I2c {
public:

    enum class Port : uint8_t {
        Num0 = 0,
        Num1 = 1,
    };

    enum class Pull : uint8_t {
        Disable = 0,
        Up,
    };

    struct Config {
        Port port = Port::Num0;
        int32_t sdaPin = -1;
        int32_t sclPin = -1;
        Pull sdaPull = Pull::Disable;
        Pull sclPull = Pull::Disable;
        uint32_t frequencyHz = 100000;
    };

    I2c();
    void init(const Config& config);

    // timeout setter
    void setTimeoutMillis(uint32_t timeoutMillis);

    bool write(
        uint8_t deviceAddress,
        const uint8_t* data,
        uint32_t lengthBytes
    );

    bool read(
        uint8_t deviceAddress,
        uint8_t* data,
        uint32_t lengthBytes
    );

    bool writeRead(
        uint8_t deviceAddress,
        const uint8_t* writeData,
        uint32_t writeLengthBytes,
        uint8_t* readData,
        uint32_t readLengthBytes
    );

    bool isInitialized() const;

    // disallow copy and move
    I2c(const I2c&) = delete;
    I2c& operator=(const I2c&) = delete;
    I2c(I2c&&) = delete;
    I2c& operator=(I2c&&) = delete;

private:
    bool mInitialized = false;
    int32_t mSdaPin = -1;
    int32_t mSclPin = -1;
    uint32_t mFrequencyHz = 100000;
    i2c_port_t mPort = I2C_NUM_0;
    uint32_t mTimeoutMillis = 10; // default timeout

    bool performWrite(
        uint8_t deviceAddress,
        const uint8_t* data,
        uint32_t lengthBytes
    );

    bool performRead(
        uint8_t deviceAddress,
        uint8_t* data,
        uint32_t lengthBytes
    );
};

} // namespace Garbox
