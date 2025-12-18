import json

def print_section(text: str, total_length: int = 60):
    core = f" {text.strip()} "
    core_len = len(core)

    if core_len > total_length:
        return core  # or raise an error

    pad = total_length - core_len
    left = pad // 2
    right = pad - left

    print("\n" + "=" * left + core + "=" * right + "\n")


def print_sep(char:str = "=", total_length: int = 60):
    print("")
    print(char*total_length)
    print("")

def decycle(obj, seen=None):
    if seen is None:
        seen = set()

    # immutable primitives: leave untouched
    if isinstance(obj, (int, float, bool, str, bytes, type(None))):
        return obj

    oid = id(obj)
    if oid in seen:
        return f"<ref>"
    seen.add(oid)

    if isinstance(obj, dict):
        return {k: decycle(v, seen) for k, v in obj.items()}

    if isinstance(obj, list):
        return [decycle(i, seen) for i in obj]

    # optionally handle other containers, or fall back:
    return obj


def extract_dict(config: dict, keys: str | list[str]):
    """
    Build a dedicated copy of the passed dict.

    Example input:
        config = { 'a': [1,2,3], 'b': [4,5,6], 'c': [7,8,9] }
        keys = ['a', 'c']
    """
    keys = ensure_list(keys)
    ex_config = {}

    for key in keys:
        ex_config[key] = config[key]

    return ex_config


def nested_get(d, keys, default=None):
    cur = d
    for k in keys:
        if isinstance(cur, dict) and k in cur:
            cur = cur[k]
        else:
            return default
    return cur


def nested_get_dot(d, path, default=None):
    cur = d
    for k in path.split("."):
        if isinstance(cur, dict) and k in cur:
            cur = cur[k]
        else:
            return default
    return cur


def print_json(data):
    print(json.dumps(data, indent=2))


def ensure_list(x):
    if x is None:
        return []
    return x if isinstance(x, list) else [x]


def ensure_dict_keys_have_suffix(in_dict: dict, suffix: str):

    out_dict = {}

    for key, value in in_dict.items():
        key_str = str(key)
        new_key = key_str if key_str.endswith(suffix) else key_str + suffix
        if new_key in out_dict:
            raise KeyError(
                f"Suffix operation would overwrite existing key '{new_key}'. "
                f"Original key: '{key_str}'"
            )
        out_dict[new_key] = value

    return out_dict
