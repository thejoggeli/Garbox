#include "McpwmPair.h"
#include "assert/Assert.h"

namespace Garbox {

McpwmPair::McpwmPair() :
    mUnit(Unit::Unit0),
    mTimer(Timer::Timer0),
    mGpioA(-1),
    mGpioB(-1),
    mFrequency(0),
    mDuty(0.0f),
    mInitialized(false),
    mEnabledA(false),
    mEnabledB(false) {
}

McpwmPair::~McpwmPair() {
    // Nothing to clean up in legacy MCPWM
}

bool McpwmPair::setup(Unit unit, Timer timer, int gpioA, int gpioB, uint32_t frequencyHz, float dutyPercent) {

    if(mInitialized) {
        AssertDebug(false, "McpwmPair already initialized");
        return false;
    }

    mUnit = unit;
    mTimer = timer;
    mGpioA = gpioA;
    mGpioB = gpioB;
    mFrequency = frequencyHz;
    mDuty = dutyPercent;

    const mcpwm_unit_t espUnit = static_cast<mcpwm_unit_t>(static_cast<int>(mUnit));
    const mcpwm_timer_t espTimer = static_cast<mcpwm_timer_t>(static_cast<int>(mTimer));

    // Initialize GPIOs
    esp_err_t result = mcpwm_gpio_init(espUnit, static_cast<mcpwm_io_signals_t>(MCPWM0A + espTimer * 2), mGpioA);
    if(result != ESP_OK) {
        AssertExit(false, "MCPWM GPIOA init failed");
        return false;
    }

    result = mcpwm_gpio_init(espUnit, static_cast<mcpwm_io_signals_t>(MCPWM0B + espTimer * 2), mGpioB);
    if(result != ESP_OK) {
        AssertExit(false, "MCPWM GPIOB init failed");
        return false;
    }

    // Configuration
    mcpwm_config_t cfg = {};
    cfg.frequency = frequencyHz;
    cfg.cmpr_a = dutyPercent;
    cfg.cmpr_b = dutyPercent;
    cfg.counter_mode = MCPWM_UP_COUNTER;
    cfg.duty_mode = MCPWM_DUTY_MODE_0;

    result = mcpwm_init(espUnit, espTimer, &cfg);
    if(result != ESP_OK) {
        AssertExit(false, "MCPWM init failed");
        return false;
    }

    // A = normal polarity
    result = mcpwm_set_duty_type(espUnit, espTimer, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
    if(result != ESP_OK) {
        AssertExit(false, "set_duty_type A failed");
        return false;
    }

    // B = inverted polarity (complementary)
    result = mcpwm_set_duty_type(espUnit, espTimer, MCPWM_OPR_B, MCPWM_DUTY_MODE_1);
    if(result != ESP_OK) {
        AssertExit(false, "set_duty_type B failed");
        return false;
    }

    // Start disabled: hold both outputs LOW
    mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_A, 0.0f);    // normal → 0% = low
    mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_B, 100.0f);  // inverted → 100% = low
    mEnabledA = false;
    mEnabledB = false;

    mInitialized = true;
    return true;
}

bool McpwmPair::setDutyPercent(float dutyPercent) {
    if(!mInitialized) {
        AssertDebug(false, "McpwmPair not initialized");
        return false;
    }

    mDuty = dutyPercent;

    const mcpwm_unit_t espUnit = static_cast<mcpwm_unit_t>(static_cast<int>(mUnit));
    const mcpwm_timer_t espTimer = static_cast<mcpwm_timer_t>(static_cast<int>(mTimer));

    if(mEnabledA)
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_A, mDuty);
    if(mEnabledB)
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_B, mDuty);

    return true;
}

bool McpwmPair::setFrequency(uint32_t frequencyHz) {
    if(!mInitialized) {
        AssertDebug(false, "McpwmPair not initialized");
        return false;
    }

    const mcpwm_unit_t espUnit = static_cast<mcpwm_unit_t>(static_cast<int>(mUnit));
    const mcpwm_timer_t espTimer = static_cast<mcpwm_timer_t>(static_cast<int>(mTimer));

    esp_err_t result = mcpwm_set_frequency(espUnit, espTimer, frequencyHz);
    if(result != ESP_OK) {
        AssertDebug(false, "MCPWM set_frequency failed");
        return false;
    }

    mFrequency = frequencyHz;
    return true;
}

bool McpwmPair::setEnabled(bool enable) {
    if(!mInitialized) {
        AssertDebug(false, "McpwmPair not initialized");
        return false;
    }

    const mcpwm_unit_t espUnit = static_cast<mcpwm_unit_t>(static_cast<int>(mUnit));
    const mcpwm_timer_t espTimer = static_cast<mcpwm_timer_t>(static_cast<int>(mTimer));

    if(enable) {
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_A, mDuty);
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_B, mDuty);
        mEnabledA = true;
        mEnabledB = true;
    } else {
        // Normal → 0% = low, Inverted → 100% = low
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_A, 0.0f);
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_B, 100.0f);
        mEnabledA = false;
        mEnabledB = false;
    }

    return true;
}

bool McpwmPair::setEnabledA(bool enable) {
    if(!mInitialized) {
        AssertDebug(false, "McpwmPair not initialized");
        return false;
    }

    const mcpwm_unit_t espUnit = static_cast<mcpwm_unit_t>(static_cast<int>(mUnit));
    const mcpwm_timer_t espTimer = static_cast<mcpwm_timer_t>(static_cast<int>(mTimer));

    if(enable) {
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_A, mDuty);
        mEnabledA = true;
    } else {
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_A, 0.0f); // normal → low
        mEnabledA = false;
    }

    return true;
}

bool McpwmPair::setEnabledB(bool enable) {
    if(!mInitialized) {
        AssertDebug(false, "McpwmPair not initialized");
        return false;
    }

    const mcpwm_unit_t espUnit = static_cast<mcpwm_unit_t>(static_cast<int>(mUnit));
    const mcpwm_timer_t espTimer = static_cast<mcpwm_timer_t>(static_cast<int>(mTimer));

    if(enable) {
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_B, mDuty);
        mEnabledB = true;
    } else {
        mcpwm_set_duty(espUnit, espTimer, MCPWM_OPR_B, 100.0f); // inverted → low
        mEnabledB = false;
    }

    return true;
}

} // namespace Garbox
