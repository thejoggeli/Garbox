from common.item import Item
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader


def generate_hardware(ctx: Context, loader: Loader):
    _generate_instances(ctx, loader)
    _generate_hardware_init(ctx, loader)


def _generate_instances(ctx: Context, loader: Loader):
    """
    - app/hardware/AdcInstances.h
    - app/hardware/AdcInstances.cpp
    - app/hardware/GpioInstances.h
    - app/hardware/GpioInstances.cpp
    - app/hardware/I2cInstances.h
    - app/hardware/I2cInstances.cpp
    - app/hardware/SpiInstances.h
    - app/hardware/SpiInstances.cpp
    - app/hardware/TimerInstances.h
    - app/hardware/TimerInstances.cpp
    - app/hardware/LedcInstances.h
    - app/hardware/LedcInstances.cpp
    """

    init_mapping = {
        "adc":        "AdcInstances",
        "gpio":       "GpioInstances",
        "i2c":        "I2cInstances",
        "spi":        "SpiInstances",
        "timer":      "TimerInstances",
        "ledc_timer": "LedcInstances",
    }

    hw_dir = ctx.app_dir / "hardware"
    config = loader.get_hardware_config()

    init_config = {"init": []}
    for key, name in init_mapping.items():
        if key in config and len(config[key]) > 0:
            init_config["init"].append(name)

    items = [
        Item(init_config, "*", hw_dir / "HardwareInit.h",   "hardware/HardwareInit.h.j2"),
        Item(init_config, "*", hw_dir / "HardwareInit.cpp", "hardware/HardwareInit.cpp.j2"),
    ]
    generate_items(ctx, items)


def _generate_hardware_init(ctx: Context, loader: Loader):
    """
    - app/hardware/HardwareInit.h
    - app/hardware/HardwareInit.cpp
    """

    hw_dir = ctx.app_dir / "hardware"
    config = loader.get_hardware_config()

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
    generate_items(ctx, items)
