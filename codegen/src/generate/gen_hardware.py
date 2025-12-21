from common.item import Item
from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader


def generate_hardware(ctx: Context, loader: Loader):
    _generate_instances(ctx, loader)
    _generate_hardware_init(ctx, loader)


def _generate_instances(ctx: Context, loader: Loader):
    """
    - generated/hardware/AdcInstances.h
    - generated/hardware/AdcInstances.cpp
    - generated/hardware/GpioInstances.h
    - generated/hardware/GpioInstances.cpp
    - generated/hardware/I2cInstances.h
    - generated/hardware/I2cInstances.cpp
    - generated/hardware/SpiInstances.h
    - generated/hardware/SpiInstances.cpp
    - generated/hardware/TimerInstances.h
    - generated/hardware/TimerInstances.cpp
    - generated/hardware/LedcInstances.h
    - generated/hardware/LedcInstances.cpp
    """

    init_mapping = {
        "adc":   "AdcInstances",
        "gpio":  "GpioInstances",
        "i2c":   "I2cInstances",
        "spi":   "SpiInstances",
        "timer": "TimerInstances",
        "ledc":  "LedcInstances",
    }

    hw_dir = ctx.gen_dir / "hardware"
    config = loader.config["hardware"]

    init_config = {"init": []}
    for key, name in init_mapping.items():
        if key in config and len(config[key]) > 0:
            init_config["init"].append(name)

    items = [
        Item(init_config, hw_dir / "HardwareInit.h",   "hardware/HardwareInit.h.j2"),
        Item(init_config, hw_dir / "HardwareInit.cpp", "hardware/HardwareInit.cpp.j2"),
    ]
    generate_items(ctx, items)


def _generate_hardware_init(ctx: Context, loader: Loader):
    """
    - generated/hardware/HardwareInit.h
    - generated/hardware/HardwareInit.cpp
    """

    hw_dir = ctx.gen_dir / "hardware"
    config = loader.config["hardware"]

    items = [
        Item(config, hw_dir/"AdcInstances.h",     "hardware/AdcInstances.h.j2"),
        Item(config, hw_dir/"AdcInstances.cpp",   "hardware/AdcInstances.cpp.j2"),
        Item(config, hw_dir/"GpioInstances.h",    "hardware/GpioInstances.h.j2"),
        Item(config, hw_dir/"GpioInstances.cpp",  "hardware/GpioInstances.cpp.j2"),
        Item(config, hw_dir/"I2cInstances.h",     "hardware/I2cInstances.h.j2"),
        Item(config, hw_dir/"I2cInstances.cpp",   "hardware/I2cInstances.cpp.j2"),
        Item(config, hw_dir/"SpiInstances.h",     "hardware/SpiInstances.h.j2"),
        Item(config, hw_dir/"SpiInstances.cpp",   "hardware/SpiInstances.cpp.j2"),
        Item(config, hw_dir/"TimerInstances.h",   "hardware/TimerInstances.h.j2"),
        Item(config, hw_dir/"TimerInstances.cpp", "hardware/TimerInstances.cpp.j2"),
        Item(config, hw_dir/"LedcInstances.h",    "hardware/LedcInstances.h.j2"),
        Item(config, hw_dir/"LedcInstances.cpp",  "hardware/LedcInstances.cpp.j2"),
    ]
    generate_items(ctx, items)
