#include "St7789v.h"

#define GarboxDebugStv7789 1

#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/hardware/ledc/LedcChannel.h"
#include "core/time/Time.h"
#include "core/util/helpers/ByteUtils.h"
#include "core/util/function/default/GammaFunctions.h"

#if GarboxDebugStv7789
#include "core/log/Log.h"
#endif

namespace Garbox {

using Command = St7789vHelper::Command;
using ColorMode = St7789vHelper::ColorMode;

St7789v::St7789v(Gpio& gpioRst, Gpio& gpioDc, Gpio& gpioCs, LedcChannel& pwmBlk):
    // init members
    mGpioRst(gpioRst),
    mGpioDc(gpioDc),
    mGpioCs(gpioCs),
    mPwmBlk(pwmBlk){
    // constructor body
}

St7789v::~St7789v(){
    TriggerExit("St7789v", "std::function may be using heap");
}

void St7789v::init(uint16_t width, uint16_t height){
    AssertExit(!mInitialized, "St7789v", "already initialized");
    AssertExit(width > 0, "St7789v", "invalid width");
    AssertExit(height > 0, "St7789v", "invalid height");
    mWidth = width;
    mHeight = height;
    mPwmBlk.setDutyRaw(0);
    mGpioCs.writeLevel(false);
    mBrightness = 0.0f;
    mInitialized = true;
}

void St7789v::setSendSyncHandler(SendSyncHandler handler){
    mSendSyncHandler = handler;
}

void St7789v::setSendAsyncHandler(SendAsyncHandler handler){
    mSendAsyncHandler = handler;
}

void St7789v::setBrightness(float brightness){
    mBrightness = std::clamp(brightness, 0.0f, 1.0f);
    const MathFunctionIfc& correctionFunction = GammaFunctions::GetGamma22();
    float brightnessCorrected = correctionFunction.evaluate(brightness);
    mPwmBlk.setDutyRelative(brightnessCorrected);
}

float St7789v::getBrightness() const {
    return mBrightness;
}

void St7789v::sendReset(){
    mGpioRst.writeLevel(false);
    Time::DelayMillis(20);
    mGpioRst.writeLevel(true);
    Time::DelayMillis(120);
}

void St7789v::sendInit(){

    Command cmd;
    uint8_t data;

    // sleep out
    cmd = Command::SLPOUT;
    mGpioCs.writeLevel(false);
    sendCommand(cmd);
    mGpioCs.writeLevel(true);

    // color mode: 16-bit
    cmd = Command::COLMOD;
    data = St7789vHelper::makeCololorModeValue(ColorMode::RGB565);
    mGpioCs.writeLevel(false);
    sendCommand(cmd, data);
    mGpioCs.writeLevel(true);

    // memory access control (rotation, RGB order)
    cmd = Command::MADCTL; 
    data = St7789vHelper::makeLandscapeRGB(false, true);
    mGpioCs.writeLevel(false);
    sendCommand(cmd, data);
    mGpioCs.writeLevel(true);

    // inversion on
    cmd = Command::INVON;
    mGpioCs.writeLevel(false);
    sendCommand(cmd);
    mGpioCs.writeLevel(true);

    // display on
    cmd = Command::DISPON;
    mGpioCs.writeLevel(false);
    sendCommand(cmd);
    mGpioCs.writeLevel(true);
}

void St7789v::sendFillColor(uint16_t color){
    sendFillRectSync(0, 0, mWidth, mHeight, color);
}

void St7789v::sendDrawBufferXXYY(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint8_t* buffer, size_t sizeBytes, bool async){
    mGpioCs.writeLevel(false);
    sendXXYY(x1, x2, y1, y2);
    sendDrawBufferInner(buffer, sizeBytes, async);
    if(!async){
        mGpioCs.writeLevel(true);
    }
}

void St7789v::sendDrawBufferXYWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* buffer, size_t sizeBytes, bool async){
    mGpioCs.writeLevel(false);
    sendXYWH(x, y, w, h);
    sendDrawBufferInner(buffer, sizeBytes, async);
    if(!async){
        mGpioCs.writeLevel(true);
    }
}

void St7789v::sendDrawBufferInner(uint8_t* buffer, size_t sizeBytes, bool async){
    sendCommand(Command::RAMWR);
    sendData(buffer, sizeBytes, async);
}

void St7789v::sendFillRectSync(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    mGpioCs.writeLevel(false);
    sendXYWH(x, y, w, h);
    sendCommand(Command::RAMWR); // sync

    // Fill with the chosen color
    uint32_t const pixelCount = (uint32_t)w * h;
    constexpr size_t blockPixels = 64;
    uint16_t block[blockPixels];
    for (size_t i = 0; i < blockPixels; ++i){
        block[i] = __builtin_bswap16(color); // ensure MSB first
    }

    uint32_t remainingPixels = pixelCount; // remaining pixels
    while (remainingPixels){
        const size_t currentPixels = std::min(remainingPixels, (uint32_t)blockPixels);
        const size_t currentBytes =  currentPixels * BytesPerPixel;
        sendData((uint8_t*)block, currentBytes, false); // sync
        remainingPixels -= currentPixels;
    }
    mGpioCs.writeLevel(true);
}

void St7789v::sendXYWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h){
    const uint16_t x1 = x;
    const uint16_t x2 = x + w - 1;
    const uint16_t y1 = y;
    const uint16_t y2 = y + h - 1;
    sendXXYY(x1, x2, y1, y2);
}

void St7789v::sendXXYY(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){
    Command cmd;
    uint8_t data[4];

    // send column address set 
    cmd = Command::CASET;
    data[0] = ByteUtils::GetByte1(x1);
    data[1] = ByteUtils::GetByte0(x1);
    data[2] = ByteUtils::GetByte1(x2);
    data[3] = ByteUtils::GetByte0(x2);
    sendCommand(cmd, data, 4); // send sync (async overhead not worth it)

    // Row address set
    cmd = Command::RASET;
    data[0] = ByteUtils::GetByte1(y1);
    data[1] = ByteUtils::GetByte0(y1);
    data[2] = ByteUtils::GetByte1(y2);
    data[3] = ByteUtils::GetByte0(y2);
    sendCommand(cmd, data, 4); // send sync (async overhead not worth it)
}

void St7789v::sendCommand(St7789vHelper::Command cmd, bool async){
    sendCommandInner(cmd, nullptr, 0, async);
}

void St7789v::sendCommand(St7789vHelper::Command cmd, uint8_t value, bool async){
    sendCommandInner(cmd, &value, 1, async);
}

void St7789v::sendCommand(St7789vHelper::Command cmd, uint8_t* buffer, size_t sizeBytes, bool async){
    sendCommandInner(cmd, buffer, sizeBytes, async);
}

void St7789v::sendCommandInner(St7789vHelper::Command cmd, uint8_t* buffer, size_t sizeBytes, bool async){
    if(!mInitialized){
        TriggerDebug("St7789v", "not initialized");
        return;
    }

    // send command
    const uint8_t cmdval = static_cast<uint8_t>(cmd);
    mGpioDc.writeLevel(false);
    sendData(&cmdval, 1, false);
    mGpioDc.writeLevel(true);

    // check if there is data to send
    if(buffer == nullptr || sizeBytes == 0){
        return;
    }

    // send data
    sendData(buffer, sizeBytes, async);
}

void St7789v::sendData(const uint8_t* buffer, size_t sizeBytes, bool async){
    if(!mSendSyncHandler){
        TriggerDebug("St7789v", "not send sync handler set");
        return;
    }
    if(async && mSendAsyncHandler){
        mSendAsyncHandler(buffer, sizeBytes);
    }
    else {
        mSendSyncHandler(buffer, sizeBytes);
    }
}

void St7789v::onSendAsyncComplete(){
    mGpioCs.writeLevel(true);
}

uint16_t St7789v::getWidth() const {
    return mWidth;
}

uint16_t St7789v::getHeight() const {
    return mHeight;
}

} // namespace Garbox
