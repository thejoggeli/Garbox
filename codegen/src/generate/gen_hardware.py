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
    items = [
        Item(config, keys=["adc"],   meta_key="adc_meta",   out_path=hw_dir/"AdcInstances.h",     template_path="hardware/AdcInstances.h.j2"),
        Item(config, keys=["adc"],   meta_key="adc_meta",   out_path=hw_dir/"AdcInstances.cpp",   template_path="hardware/AdcInstances.cpp.j2"),
        Item(config, keys=["gpio"],  meta_key="gpio_meta",  out_path=hw_dir/"GpioInstances.h",    template_path="hardware/GpioInstances.h.j2"),
        Item(config, keys=["gpio"],  meta_key="gpio_meta",  out_path=hw_dir/"GpioInstances.cpp",  template_path="hardware/GpioInstances.cpp.j2"),
        Item(config, keys=["i2c"],   meta_key="i2c_meta",   out_path=hw_dir/"I2cInstances.h",     template_path="hardware/I2cInstances.h.j2"),
        Item(config, keys=["i2c"],   meta_key="i2c_meta",   out_path=hw_dir/"I2cInstances.cpp",   template_path="hardware/I2cInstances.cpp.j2"),
        Item(config, keys=["spi"],   meta_key="spi_meta",   out_path=hw_dir/"SpiInstances.h",     template_path="hardware/SpiInstances.h.j2"),
        Item(config, keys=["spi"],   meta_key="spi_meta",   out_path=hw_dir/"SpiInstances.cpp",   template_path="hardware/SpiInstances.cpp.j2"),
        Item(config, keys=["timer"], meta_key="timer_meta", out_path=hw_dir/"TimerInstances.h",   template_path="hardware/TimerInstances.h.j2"),
        Item(config, keys=["timer"], meta_key="timer_meta", out_path=hw_dir/"TimerInstances.cpp", template_path="hardware/TimerInstances.cpp.j2"),
        Item(config, keys=["ledc_channel", "ledc_timer"], meta_key="ledc_meta", out_path=hw_dir/"LedcInstances.h",   template_path="hardware/LedcInstances.h.j2"),
        Item(config, keys=["ledc_channel", "ledc_timer"], meta_key="ledc_meta", out_path=hw_dir/"LedcInstances.cpp", template_path="hardware/LedcInstances.cpp.j2"),
    ]

    # HardwareInit files
    init_mapping = {
        "adc":        "AdcInstances",
        "gpio":       "GpioInstances",
        "i2c":        "I2cInstances",
        "spi":        "SpiInstances",
        "timer":      "TimerInstances",
        "ledc_timer": "LedcInstances",
    }

    init_config = {"init": []}
    for key, name in init_mapping.items():
        if key in config and len(config[key]) > 0:
            init_config["init"].append(name)

    items.append(Item(init_config, "*", hw_dir / "HardwareInit.h",   "hardware/HardwareInit.h.j2"))
    items.append(Item(init_config, "*", hw_dir / "HardwareInit.cpp", "hardware/HardwareInit.cpp.j2"))

    # generate to src_dir
    generate_items(ctx, items)
