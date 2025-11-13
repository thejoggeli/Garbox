#pragma once

namespace Garbox {

class ButtonIfc;
class Display;
class Fan;
class Heatpad;
class PiezoPlayer;

class StatusLeds;
class RgbLed;

class PartsProvider {
public:

    static Fan& GetFan(); 
    static Heatpad& GetHeatpad(); 
    static Display& GetDisplay(); 
    static PiezoPlayer& GetPiezoPlayer(); 
    static ButtonIfc& GetEncoderButton(); 

    static StatusLeds& GetStatusLeds(); 
    static RgbLed& GetRgbLed();

};

} // namespace