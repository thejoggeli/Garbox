#include "PiezoSequences.h"

#include "core/time/TimeLiterals.h"

namespace Garbox {

const ToneSequence& PiezoSequences::GetStartup(){
    static constexpr Tone tones[] = {
        Tone(100_ms, 1000), Tone( 50_ms),
        Tone(250_ms, 1500),
        Tone(100_ms, 1250),
        Tone(150_ms, 1000),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetButton(){
    static constexpr Tone tones[] = {
        Tone(200_ms, 1000, 1000, 0.2f ), Tone( 50_ms),
        Tone(200_ms, 1000, 1000, 0.4f ), Tone( 50_ms),
        Tone(200_ms, 1000, 1000, 0.6f ), Tone( 50_ms),
        Tone(200_ms, 1000, 1000, 0.8f ), Tone( 50_ms),
        Tone(200_ms, 1000, 2000, 0.25f), Tone( 50_ms),
        Tone(200_ms, 1000, 1500, 0.5f ), Tone( 50_ms),
        Tone(200_ms, 1000, 1000, 0.75f),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetHelixUp(){
    static constexpr Tone tones[] = {
        Tone(80_ms, 1000, 1000, 0.25f),
        Tone(80_ms, 1000, 2000, 0.25f),
        Tone(80_ms, 1000, 3000, 0.25f),
        Tone(80_ms, 1000, 4000, 0.25f),
        Tone(80_ms, 1000, 5000, 0.25f),
        Tone(80_ms, 1000, 6000, 0.25f),
        Tone(80_ms, 1000, 7000, 0.25f),
        Tone(80_ms, 1000, 8000, 0.25f),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetHelixDown(){
    static constexpr Tone tones[] = {
        Tone(80_ms, 8000, 1000, 0.25f),
        Tone(80_ms, 7000, 1000, 0.25f),
        Tone(80_ms, 6000, 1000, 0.25f),
        Tone(80_ms, 5000, 1000, 0.25f),
        Tone(80_ms, 4000, 1000, 0.25f),
        Tone(80_ms, 3000, 1000, 0.25f),
        Tone(80_ms, 2000, 1000, 0.25f),
        Tone(80_ms, 1000, 1000, 0.25f),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetInterpolated1(){
    static constexpr Tone tones[] = {
        Tone(1000_ms, 1000, 2000),
        Tone( 500_ms, 2000, 1500),
        Tone(1000_ms, 1500, 2500),
        Tone( 200_ms), Tone( 400_ms, 3000),
        Tone( 200_ms), Tone( 400_ms, 2250),
        Tone( 200_ms), Tone( 400_ms, 1500),
        Tone( 200_ms), Tone( 800_ms, 1500, 750),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetInterpolated2(){
    static constexpr Tone tones[] = {
        Tone(1000_ms, 1000),
        Tone( 250_ms, 1000, 2000),
        Tone(1000_ms, 2000),
        Tone(1000_ms, 1000, 500),
        Tone(1000_ms, 500),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetFanStalled(){
    static constexpr Tone tones[] = {
        Tone(100_ms, 1000, 1000).duty(0.75f),
        Tone(100_ms),
        Tone(100_ms, 1000, 1000).duty(0.25f),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetFanEnabled(){
    static constexpr Tone tones[] = {
        Tone(200_ms, 500, 750).duty(0.25f),
        Tone(200_ms, 750, 2000).duty(0.5f),
        Tone(200_ms, 1750, 5000).duty(0.75f),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

const ToneSequence& PiezoSequences::GetFanDisabled(){
    static constexpr Tone tones[] = {
        Tone(200_ms, 5000, 1750).duty(0.75f),
        Tone(200_ms, 2000, 750).duty(0.5f),
        Tone(200_ms, 750, 500).duty(0.25f),
    };
    static constexpr ToneSequence sequence = ToneSequence::from(tones);
    return sequence;
}

} // namespace Garbox
