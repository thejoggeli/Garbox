from common.util import ( 
    ensure_dict_keys_have_suffix, 
    ensure_str_has_suffix,
)

def parse_hardware_config(config):

    # enforce required suffix rules
    hardware = config["hardware"]
    hardware["ledc"]["instances"]["timers"] = ensure_dict_keys_have_suffix(hardware["ledc"]["instances"]["timers"], "Timer")
    hardware["ledc"]["instances"]["channels"] = ensure_dict_keys_have_suffix(hardware["ledc"]["instances"]["channels"], "Channel")

    # ensure timer bindings in ledc channels have 'Timer' suffix
    for channel in hardware["ledc"]["instances"]["channels"].values():
        channel["timer"] = ensure_str_has_suffix(channel["timer"], "Timer")
