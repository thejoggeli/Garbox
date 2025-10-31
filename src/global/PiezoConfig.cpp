#include "PiezoConfig.h"

#include "core/time/TimeLiterals.h"

namespace Garbox {

static constexpr Tone startupTones[] = {
    Tone(1000, 100_ms),
    Tone(   0,  50_ms),
    Tone(1500, 250_ms),
    Tone(1250, 100_ms),
    Tone(1000, 150_ms),
};

static constexpr Tone buttonTones[] = {
    Tone(1000, 100_ms),
};

const ToneSequence PiezoConfig::startupSequence(startupTones, sizeof(startupTones) / sizeof(startupTones[0]));
const ToneSequence PiezoConfig::buttonSequence(buttonTones, sizeof(buttonTones) / sizeof(buttonTones[0]));

} // namespace
