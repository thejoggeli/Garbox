#include "PiezoConfig.h"

#include "core/time/TimeLiterals.h"

namespace Garbox {

static constexpr Tone StartupTones[] = {
    Tone(100_ms, 1000),
    Tone( 50_ms,    0),
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

const ToneSequence PiezoConfig::StartupSequence(StartupTones, sizeof(StartupTones) / sizeof(StartupTones[0]));
const ToneSequence PiezoConfig::ButtonSequence(ButtonTones, sizeof(ButtonTones) / sizeof(ButtonTones[0]));

} // namespace
