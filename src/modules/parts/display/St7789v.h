#pragma once

#include <functional>
#include "St7789vHelper.h"

namespace Garbox {

class Gpio;
class LedcChannel;

class St7789v {
public:

    using SendSyncHandler = std::function<void(const uint8_t* data, size_t numBytes)>;
    using SendAsyncHandler = std::function<void(const uint8_t* data, size_t numBytes)>;

    St7789v(Gpio& gpioRst, Gpio& gpioDc, Gpio& gpioCs, LedcChannel& pwmBlk);
    ~St7789v();

    void init(uint16_t width, uint16_t height);

    void setSendSyncHandler(SendSyncHandler handler);
    void setSendAsyncHandler(SendAsyncHandler handler);

    void setBrightness(float brightness);

    uint16_t getWidth() const;
    uint16_t getHeight() const;

    void sendReset();
    void sendInit();

    void sendFillColor(uint16_t color);
    void sendFillRectSync(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void sendDrawBufferXYWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* buffer, size_t sizeBytes, bool async = false);
    void sendDrawBufferXXYY(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint8_t* buffer, size_t sizeBytes, bool async = false);

    void onSendAsyncComplete();

    // disallow copy and move 
    St7789v(const St7789v&) = delete;
    St7789v& operator=(const St7789v&) = delete;
    St7789v(St7789v&&) = delete;
    St7789v& operator=(St7789v&&) = delete;

private:

    static constexpr uint8_t BytesPerPixel = 2;

    Gpio& mGpioRst;
    Gpio& mGpioDc;
    Gpio& mGpioCs;
    LedcChannel& mPwmBlk;

    uint16_t mWidth = 0;
    uint16_t mHeight = 0;
    bool mBrightness = 0.0f;

    SendSyncHandler mSendSyncHandler = nullptr;
    SendAsyncHandler mSendAsyncHandler = nullptr;

    bool mInitialized = false;

    void sendCommand(St7789vHelper::Command cmd, bool async = false);
    void sendCommand(St7789vHelper::Command cmd, uint8_t value, bool async = false);
    void sendCommand(St7789vHelper::Command cmd, uint8_t* buffer, size_t sizeBytes, bool async = false);
    void sendCommandInner(St7789vHelper::Command cmd, uint8_t* buffer, size_t sizeBytes, bool async);

    void sendXYWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void sendXXYY(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);

    void sendDrawBufferInner(uint8_t* buffer, size_t sizeBytes, bool async);
    void sendData(const uint8_t* buffer, size_t sizeBytes, bool async);
};

} // namespace
