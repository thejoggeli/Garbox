#pragma once

#include "app/types/StatusLedId.h"

namespace Garbox {

class ButtonIfc;
class Display;
class Fan;
class Heatpad;
class PiezoPlayer;
class Sht31;

class AnimatedLed;
class AnimatedLedGroup;
class RgbLed;

class LvglContext;

class PartsProvider {
public:

    static void Init();

    static Fan& GetFan(); 
    static Heatpad& GetHeatpad(); 
    static Display& GetDisplay(); 
    static PiezoPlayer& GetPiezoPlayer(); 
    static ButtonIfc& GetEncoderButton(); 
    static Sht31& GetTemperatureSensor();

    static AnimatedLed& GetStatusLed(StatusLedId id);
    static AnimatedLedGroup& GetStatusLeds();
    static RgbLed& GetRgbLed();

    static LvglContext& GetLvglContext();

};

} // namespace