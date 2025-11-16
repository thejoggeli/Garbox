#pragma once

namespace Garbox {

class PiezoPlayer;
class StatusLeds;
class AnimatedLed;

class StartupSequence {
public:

    StartupSequence();
    void run();

private: 

    PiezoPlayer& mPiezoPlayer;
    StatusLeds& mStatusLeds;
    AnimatedLed& mHeartbeatLed;

    void playSweepIn();
    void playSweepOut();
    void waitUntilSweepComplete();

};

} // namespace