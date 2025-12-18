#pragma once

namespace Garbox {

class AnimatedLedGroup;
class AnimatedLed;
class PiezoPlayer;

class StartupSequence {
public:

    StartupSequence();
    void run();

private: 

    PiezoPlayer& mPiezoPlayer;
    AnimatedLedGroup& mStatusLeds;
    AnimatedLed& mHeartbeatLed;

    void playSweepIn();
    void playSweepOut();
    void waitUntilSweepComplete();

};

} // namespace