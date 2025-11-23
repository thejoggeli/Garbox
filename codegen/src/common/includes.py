from sortedcontainers import SortedSet


def extract_includes(config: dict, extension: str, remove: bool = True):
    """
    Collects include_h or include_cpp entries from all config entries.
    Returns a sorted, deduplicated list.
    Optionally removes include_h/include_cpp fields from config.
    """

    if extension == ".h":
        include_key = "include_h"
    elif extension == ".cpp":
        include_key = "include_cpp"
    else:
        include_key = None

    includes_list = []
    if include_key is not None:
        includes_set = SortedSet()
        for config_entry in config.values():
            if include_key in config_entry:
                for include in config_entry[include_key]:
                    includes_set.add(include)
        includes_list = list(includes_set)

    if remove:
        for config_entry in config.values():
            if "include_h" in config_entry:
                del config_entry["include_h"]
            if "include_cpp" in config_entry:
                del config_entry["include_cpp"]

    return includes_list
