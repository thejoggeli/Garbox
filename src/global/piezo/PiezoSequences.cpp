#include "PiezoSequences.h"
#include "core/time/TimeLiterals.h"

namespace Garbox {

static constexpr Tone StartupTones[] = {
    Tone(100_ms, 1000), Tone( 50_ms),
    Tone(250_ms, 1500),
    Tone(100_ms, 1250),
    Tone(150_ms, 1000),
};

static constexpr Tone ButtonTones[] = {
    Tone(200_ms, 1000, 1000, 0.2f ), Tone( 50_ms),
    Tone(200_ms, 1000, 1000, 0.4f ), Tone( 50_ms),
    Tone(200_ms, 1000, 1000, 0.6f ), Tone( 50_ms),
    Tone(200_ms, 1000, 1000, 0.8f ), Tone( 50_ms),
    Tone(200_ms, 1000, 2000, 0.25f), Tone( 50_ms),
    Tone(200_ms, 1000, 1500, 0.5f ), Tone( 50_ms),
    Tone(200_ms, 1000, 1000, 0.75f),
};

static constexpr Tone HelixTones[] = {
    Tone(80_ms, 1000, 1000, 0.25f),
    Tone(80_ms, 1000, 2000, 0.25f),
    Tone(80_ms, 1000, 3000, 0.25f),
    Tone(80_ms, 1000, 4000, 0.25f),
    Tone(80_ms, 1000, 5000, 0.25f),
    Tone(80_ms, 1000, 6000, 0.25f),
    Tone(80_ms, 1000, 7000, 0.25f),
    Tone(80_ms, 1000, 8000, 0.25f),
};

static constexpr Tone InterpolatedTones[] = {
    Tone(1000_ms, 1000, 2000),
    Tone( 500_ms, 2000, 1500),
    Tone(1000_ms, 1500, 2500),
    Tone( 200_ms), Tone( 400_ms, 3000),
    Tone( 200_ms), Tone( 400_ms, 2250),
    Tone( 200_ms), Tone( 400_ms, 1500),
    Tone( 200_ms), Tone( 800_ms, 1500, 750),
};

static constexpr Tone InterpolatedTones2[] = {
    Tone(1000_ms, 1000),
    Tone( 250_ms, 1000, 2000),
    Tone(1000_ms, 2000),
    Tone(1000_ms, 1000, 500),
    Tone(1000_ms, 500),
};

// clean, constexpr, macro-free
const ToneSequence PiezoSequences::Startup = ToneSequence::from(StartupTones);
const ToneSequence PiezoSequences::Button = ToneSequence::from(ButtonTones);
const ToneSequence PiezoSequences::Helix = ToneSequence::from(HelixTones);
const ToneSequence PiezoSequences::Interpolated1 = ToneSequence::from(InterpolatedTones);
const ToneSequence PiezoSequences::Interpolated2 = ToneSequence::from(InterpolatedTones2);

} // namespace Garbox
