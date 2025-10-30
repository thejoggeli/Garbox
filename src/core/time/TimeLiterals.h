#pragma once

#include <cstdint>

namespace Garbox {

constexpr unsigned int operator"" _us(unsigned long long v) { return v; }
constexpr float operator"" _us(long double v) { return static_cast<float>(v); }

constexpr unsigned int operator"" _ms(unsigned long long v) { return static_cast<unsigned int>(v * 1000ULL); }
constexpr float operator"" _ms(long double v) { return static_cast<float>(v * 1000.0L); }

constexpr unsigned int operator"" _s(unsigned long long v) { return static_cast<unsigned int>(v * 1000000ULL); }
constexpr float operator"" _s(long double v) { return static_cast<float>(v * 1000000.0L); }

} // namespace
