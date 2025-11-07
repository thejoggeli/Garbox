#pragma once
#include <cstdint>

namespace Garbox {

/**
 * ST7789V helper for command register constants and configuration values.
 * Provides:
 *   - Named command enums (with documentation)
 *   - Type-safe color mode values
 *   - Precomputed MADCTL orientation bytes
 */
class St7789vHelper {
public:
    // -------------------------------------------------------------
    // Command register definitions (DC = 0 when sending)
    // -------------------------------------------------------------
    enum class Command : uint8_t {
        NOP        = 0x00, // No operation
        SWRESET    = 0x01, // Software reset (same as hardware RST)
        RDDID      = 0x04, // Read display ID
        RDDST      = 0x09, // Read display status

        SLPIN      = 0x10, // Enter sleep mode
        SLPOUT     = 0x11, // Exit sleep mode (wait ≥120 ms)
        PTLON      = 0x12, // Partial mode on
        NORON      = 0x13, // Normal display mode on

        INVOFF     = 0x20, // Display inversion off
        INVON      = 0x21, // Display inversion on (recommended for most ST7789V)

        DISPOFF    = 0x28, // Display off (blank)
        DISPON     = 0x29, // Display on

        CASET      = 0x2A, // Column address set (X start/end)
        RASET      = 0x2B, // Row address set (Y start/end)
        RAMWR      = 0x2C, // Memory write (start pixel stream)
        RAMRD      = 0x2E, // Memory read

        PTLAR      = 0x30, // Partial area definition
        VSCRDEF    = 0x33, // Vertical scrolling area definition
        TEOFF      = 0x34, // Tearing effect line off
        TEON       = 0x35, // Tearing effect line on
        MADCTL     = 0x36, // Memory access control (rotation, mirroring, color order)
        VSCSAD     = 0x37, // Vertical scroll start address

        IDMOFF     = 0x38, // Idle mode off
        IDMON      = 0x39, // Idle mode on
        COLMOD     = 0x3A, // Interface pixel format (e.g. 16-bit 5-6-5 RGB)

        WRDISBV    = 0x51, // Write display brightness (PWM)
        RDDISBV    = 0x52, // Read display brightness
        WRCTRLD    = 0x53, // Write CTRL display (backlight control)
        RDCTRLD    = 0x54, // Read CTRL display
        WRCACE     = 0x55, // Write content adaptive brightness control
        RDCABC     = 0x56, // Read CABC
        WRCABCMB   = 0x5E, // Write CABC minimum brightness
        RDCABCMB   = 0x5F, // Read CABC minimum brightness

        RDID1      = 0xDA, // Read ID1
        RDID2      = 0xDB, // Read ID2
        RDID3      = 0xDC  // Read ID3
    };

    // -------------------------------------------------------------
    // COLMOD (color depth)
    // -------------------------------------------------------------
    enum class ColorMode : uint8_t {
        RGB444 = 0x03, // 12-bit color (4 bits per channel)
        RGB565 = 0x55, // 16-bit color (5-6-5)
        RGB666 = 0x66  // 18-bit color (6 bits per channel)
    };

    static constexpr uint8_t makeCololorModeValue(ColorMode mode) {
        return static_cast<uint8_t>(mode);
    }

    // -------------------------------------------------------------
    // MADCTL bit masks
    // -------------------------------------------------------------
    static constexpr uint8_t MY  = 0b1000'0000; // Row address order (flip Y)
    static constexpr uint8_t MX  = 0b0100'0000; // Column address order (flip X)
    static constexpr uint8_t MV  = 0b0010'0000; // Row/column exchange (swap X/Y)
    static constexpr uint8_t ML  = 0b0001'0000; // Vertical refresh order
    static constexpr uint8_t BGR = 0b0000'1000; // Color order select (1=BGR, 0=RGB)
    static constexpr uint8_t MH  = 0b0000'0100; // Horizontal refresh order

    // -------------------------------------------------------------
    // Ready-to-use MADCTL preset values
    // -------------------------------------------------------------
    // Note: In landscape mode (MV=1), MX and MY effectively swap roles.
    // Depending on panel wiring, flipX/flipY may need to be swapped
    // if the image flips in the wrong direction.

    static constexpr uint8_t makePortraitRGB(bool flipX = false, bool flipY = false) {
        return (flipY ? MY : 0) | (flipX ? MX : 0);
    }

    static constexpr uint8_t makePortraitBGR(bool flipX = false, bool flipY = false) {
        return (flipY ? MY : 0) | (flipX ? MX : 0) | BGR;
    }

    static constexpr uint8_t makeLandscapeRGB(bool flipX = false, bool flipY = false) {
        return MV | (flipY ? MX : 0) | (flipX ? MY : 0);
    }

    static constexpr uint8_t makeLandscapeBGR(bool flipX = false, bool flipY = false) {
        return MV | (flipY ? MX : 0) | (flipX ? MY : 0) | BGR;
    }
};

} // namespace Garbox
