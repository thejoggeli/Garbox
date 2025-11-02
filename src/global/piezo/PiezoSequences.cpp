#include "PiezoSequences.h"
#include "core/time/TimeLiterals.h"

namespace Garbox {

// Helper to automatically infer array size and build a ToneSequence
template <size_t N>
constexpr ToneSequence makeSequence(const Tone (&tones)[N]) {
    return ToneSequence(tones, N);
}

// Startup
static constexpr Tone StartupTones[] = {
    Tone(100_ms, 1000),
    Tone(50_ms),
    Tone(250_ms, 1500),
    Tone(100_ms, 1250),
    Tone(150_ms, 1000),
};
const ToneSequence PiezoSequences::Startup = makeSequence(StartupTones);

// Button
static constexpr Tone ButtonTones[] = {
    Tone(500_ms, 1000),
    Tone(500_ms, 1500),
    Tone(250_ms, 1000),
    Tone(250_ms, 2000),
    Tone(125_ms, 1500),
    Tone(125_ms, 2500),
};
const ToneSequence PiezoSequences::Button = makeSequence(ButtonTones);

// Helix
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
const ToneSequence PiezoSequences::Helix = makeSequence(HelixTones);

// Interpolated
static constexpr Tone Interpolated1Tones[] = {
    Tone(1000_ms, 1000, 2000),
    Tone(500_ms, 2000, 1500),
    Tone(1000_ms, 1500, 2500),
    Tone(200_ms, 0),
    Tone(400_ms, 3000),
    Tone(200_ms, 0),
    Tone(400_ms, 2250),
    Tone(200_ms, 0),
    Tone(400_ms, 1500),
    Tone(200_ms, 0),
    Tone(800_ms, 1500, 750),
};
const ToneSequence PiezoSequences::Interpolated1  = makeSequence(Interpolated1Tones);

// Interpolated
static constexpr Tone InterpolatedTones2[] = {
    Tone(1000_ms, 1000),
    Tone(250_ms, 1000, 2000),
    Tone(1000_ms, 2000),
    Tone(1000_ms, 1000, 500),
    Tone(1000_ms, 500),
};
const ToneSequence PiezoSequences::Interpolated2  = makeSequence(InterpolatedTones2);

} // namespace Garbox
