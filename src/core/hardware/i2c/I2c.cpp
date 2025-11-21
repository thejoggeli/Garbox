#include "I2c.h"

#include "core/assert/Assert.h"
#include "driver/i2c.h"

namespace Garbox {

I2c::I2c(){
    // nothing to do
}

void I2c::setTimeoutMillis(uint32_t timeoutMillis){
    mTimeoutMillis = timeoutMillis;
}

void I2c::init(const Config& config){
    AssertExit(!mInitialized, "I2c", "already initialized");
    AssertExit((config.pinSda >= 0), "I2c", "invalid sda pin");
    AssertExit((config.pinScl >= 0), "I2c", "invalid scl pin");

    mPinSda = config.pinSda;
    mPinScl = config.pinScl;
    mFrequencyHz = config.frequencyHz;
    mPort = (config.port == Port::Num0) ? I2C_NUM_0 : I2C_NUM_1;

    i2c_config_t i2cConfig = {};
    i2cConfig.mode = I2C_MODE_MASTER;
    i2cConfig.sda_io_num = mPinSda;
    i2cConfig.scl_io_num = mPinScl;
    i2cConfig.sda_pullup_en = (config.pullSda == Pull::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    i2cConfig.scl_pullup_en = (config.pullScl == Pull::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    i2cConfig.master.clk_speed = mFrequencyHz;

    if(i2c_param_config(mPort, &i2cConfig) != ESP_OK){
        TriggerExit("I2c", "i2c_param_config failed");
    }

    if(i2c_driver_install(mPort, i2cConfig.mode, 0, 0, 0) != ESP_OK){
        TriggerExit("I2c", "i2c_driver_install failed");
    }

    mInitialized = true;
}

bool I2c::performWrite(
    uint8_t deviceAddress,
    const uint8_t* data,
    uint32_t lengthBytes
){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if(cmd == nullptr){
        TriggerDebug("I2c", "cmd creation failed");
        return false;
    }

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data, lengthBytes, true);
    i2c_master_stop(cmd);

    const esp_err_t result = i2c_master_cmd_begin(
        mPort,
        cmd,
        mTimeoutMillis / portTICK_PERIOD_MS
    );

    i2c_cmd_link_delete(cmd);

    const bool ok = (result == ESP_OK);
    return ok;
}

bool I2c::performRead(
    uint8_t deviceAddress,
    uint8_t* data,
    uint32_t lengthBytes
){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if(cmd == nullptr){
        TriggerDebug("I2c", "cmd creation failed");
        return false;
    }

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_READ, true);
    if(lengthBytes > 1){
        i2c_master_read(cmd, data, lengthBytes - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + (lengthBytes - 1), I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    const esp_err_t result = i2c_master_cmd_begin(
        mPort,
        cmd,
        mTimeoutMillis / portTICK_PERIOD_MS
    );

    i2c_cmd_link_delete(cmd);

    const bool ok = (result == ESP_OK);
    return ok;
}

bool I2c::write(
    uint8_t deviceAddress,
    const uint8_t* data,
    uint32_t lengthBytes
){
    if(!mInitialized){
        TriggerDebug("I2c", "not initialized");
        return false;
    }

    const bool ok = performWrite(deviceAddress, data, lengthBytes);
    return ok;
}

bool I2c::read(
    uint8_t deviceAddress,
    uint8_t* data,
    uint32_t lengthBytes
){
    if(!mInitialized){
        TriggerDebug("I2c", "not initialized");
        return false;
    }

    const bool ok = performRead(deviceAddress, data, lengthBytes);
    return ok;
}

bool I2c::writeRead(
    uint8_t deviceAddress,
    const uint8_t* writeData,
    uint32_t writeLengthBytes,
    uint8_t* readData,
    uint32_t readLengthBytes
){
    if(!mInitialized){
        TriggerDebug("I2c", "not initialized");
        return false;
    }

    const bool writeOk = performWrite(deviceAddress, writeData, writeLengthBytes);
    if(!writeOk){
        return false;
    }

    const bool readOk = performRead(deviceAddress, readData, readLengthBytes);
    return readOk;
}

bool I2c::isInitialized() const {
    return mInitialized;
}

} // namespace Garbox
