#include "PiezoConfig.h"

#include "core/time/TimeLiterals.h"

#define ArraySize(arr) (sizeof(arr) / sizeof((arr)[0]))

#define DefineToneSequence(tones, seq) \
    const ToneSequence PiezoConfig::seq(tones, sizeof(tones) / sizeof(tones[0]));

namespace Garbox {

static constexpr Tone StartupTones[] = {
    Tone(100_ms, 1000),
    Tone( 50_ms),
    Tone(250_ms, 1500),
    Tone(100_ms, 1250),
    Tone(150_ms, 1000),
};

static constexpr Tone ButtonTones[] = {
    Tone(500_ms, 1000),
    Tone(500_ms, 1500),
    Tone(250_ms, 1000),
    Tone(250_ms, 2000),
    Tone(125_ms, 1500),
    Tone(125_ms, 2500),
};

static constexpr Tone HelixTones[] = {
    Tone(80_ms, 1000, 1000),
    Tone(80_ms, 1000, 2000),
    Tone(80_ms, 1000, 3000),
    Tone(80_ms, 1000, 4000),
    Tone(80_ms, 1000, 5000),
    Tone(80_ms, 1000, 6000),
};

static constexpr Tone InterpolatedTones[] = {
    Tone(1000_ms, 1000, 2000),
    Tone( 500_ms, 2000, 1500),
    Tone(1000_ms, 1500, 2500),
    Tone( 200_ms, 0),
    Tone( 400_ms, 3000),
    Tone( 200_ms, 0),
    Tone( 400_ms, 2250),
    Tone( 200_ms, 0),
    Tone( 400_ms, 1500),
    Tone( 200_ms, 0),
    Tone( 800_ms, 1500, 750),
};

static constexpr Tone InterpolatedTones2[] = {
    Tone(1000_ms, 1000),
    Tone( 250_ms, 1000, 2000),
    Tone(1000_ms, 2000),
    Tone(1000_ms, 1000, 500),
    Tone(1000_ms, 500),
};

const ToneSequence PiezoConfig::StartupSequence(StartupTones, ArraySize(StartupTones));
const ToneSequence PiezoConfig::ButtonSequence(ButtonTones, ArraySize(ButtonTones));
const ToneSequence PiezoConfig::HelixSequence(HelixTones, ArraySize(HelixTones));
const ToneSequence PiezoConfig::InterpolatedSequence(InterpolatedTones, ArraySize(InterpolatedTones));
const ToneSequence PiezoConfig::InterpolatedSequence2(InterpolatedTones2, ArraySize(InterpolatedTones2));

} // namespace
