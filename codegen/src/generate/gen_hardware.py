from common.item import Item
from common.item import Item, generate_items
from common.context import Context


def generate_hardware(ctx: Context, config: dict):
    """
    Generate all hardware related files.

    Hardware Initialization:
    - app/hardware/HardwareInit.h
    - app/hardware/HardwareInit.cpp

    ADC:
    - app/hardware/AdcInstances.h
    - app/hardware/AdcInstances.cpp

    GPIO:
    - app/hardware/GpioInstances.h
    - app/hardware/GpioInstances.cpp

    I2C:
    - app/hardware/I2cInstances.h
    - app/hardware/I2cInstances.cpp

    SPI:
    - app/hardware/SpiInstances.h
    - app/hardware/SpiInstances.cpp

    Timer:
    - app/hardware/TimerInstances.h
    - app/hardware/TimerInstances.cpp

    LEDC:
    - app/hardware/LedcInstances.h
    - app/hardware/LedcInstances.cpp
    """

    hw_dir = ctx.app_dir / "hardware"

    # list of Item objects to generate
    ledc_keys = ["ledcChannel", "ledcTimer"]
    items = [
        Item(config, ["adc"],   hw_dir / "AdcInstances.h",     "hardware/AdcInstances.h.j2"),
        Item(config, ["adc"],   hw_dir / "AdcInstances.cpp",   "hardware/AdcInstances.cpp.j2"),
        Item(config, ["gpio"],  hw_dir / "GpioInstances.h",    "hardware/GpioInstances.h.j2"),
        Item(config, ["gpio"],  hw_dir / "GpioInstances.cpp",  "hardware/GpioInstances.cpp.j2"),
        Item(config, ["i2c"],   hw_dir / "I2cInstances.h",     "hardware/I2cInstances.h.j2"),
        Item(config, ["i2c"],   hw_dir / "I2cInstances.cpp",   "hardware/I2cInstances.cpp.j2"),
        Item(config, ["spi"],   hw_dir / "SpiInstances.h",     "hardware/SpiInstances.h.j2"),
        Item(config, ["spi"],   hw_dir / "SpiInstances.cpp",   "hardware/SpiInstances.cpp.j2"),
        Item(config, ["timer"], hw_dir / "TimerInstances.h",   "hardware/TimerInstances.h.j2"),
        Item(config, ["timer"], hw_dir / "TimerInstances.cpp", "hardware/TimerInstances.cpp.j2"),
        Item(config, ledc_keys, hw_dir / "LedcInstances.h",   "hardware/LedcInstances.h.j2"),
        Item(config, ledc_keys, hw_dir / "LedcInstances.cpp", "hardware/LedcInstances.cpp.j2"),
    ]

    # HardwareInit files
    init_mapping = {
        "adc":        "AdcInstances",
        "gpio":       "GpioInstances",
        "i2c":        "I2cInstances",
        "spi":        "SpiInstances",
        "timer":      "TimerInstances",
        "ledcTimer":  "LedcInstances",
    }

    init_config = {"init": []}
    for key, name in init_mapping.items():
        if key in config and len(config[key]) > 0:
            init_config["init"].append(name)

    items.append(Item(init_config, "*", hw_dir / "HardwareInit.h",   "hardware/HardwareInit.h.j2"))
    items.append(Item(init_config, "*", hw_dir / "HardwareInit.cpp", "hardware/HardwareInit.cpp.j2"))

    # generate to src_dir
    generate_items(ctx, items)
