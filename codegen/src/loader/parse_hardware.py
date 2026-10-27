from common.util import ensure_dict_keys_have_suffix
from common.str_filters import ensure_suffix

def parse_hardware_config(config):

    # enforce required suffix rules
    hardware = config["hardware"]
    ledc = hardware.get("ledc")
    if ledc is None:
        return
    instances = ledc.setdefault("instances", {})
    instances["timers"] = ensure_dict_keys_have_suffix(instances.get("timers", {}), "Timer")
    instances["channels"] = ensure_dict_keys_have_suffix(instances.get("channels", {}), "Channel")

    # ensure timer bindings in ledc channels have 'Timer' suffix
    for channel in instances["channels"].values():
        channel["timer"] = ensure_suffix(channel["timer"], "Timer")
