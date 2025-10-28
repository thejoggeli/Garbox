#include "Gpio.h"

namespace Garbox {

Gpio::Gpio(uint32_t pin) : mPin(pin) {}

void Gpio::setMode(Mode mode) {
    pinMode(mPin, static_cast<uint8_t>(mode));
}

void Gpio::digitalWrite(bool value) {
    mState = value;
    ::digitalWrite(mPin, value ? HIGH : LOW);
}

bool Gpio::digitalRead() const {
    return ::digitalRead(mPin) == HIGH;
}

void Gpio::toggle() {
    mState = !mState;
    ::digitalWrite(mPin, mState ? HIGH : LOW);
}

bool Gpio::getDigitalState() const {
    return mState;
}

int Gpio::analogRead() const {
    return ::analogRead(mPin);
}

void Gpio::analogWrite(uint32_t value) {
    ::analogWrite(mPin, value);
}

}  // namespace
