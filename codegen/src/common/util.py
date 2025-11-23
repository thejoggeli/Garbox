import json


def print_json(data):
    print(json.dumps(data, indent=2))


def ensure_list(x):
    if x is None:
        return []
    return x if isinstance(x, list) else [x]


def ensure_suffix(yaml_config: dict, parent: str, suffix: str):
    block = yaml_config.get(parent)
    if block is None:
        raise KeyError(f"Parent key '{parent}' not found in YAML config")

    if not isinstance(block, dict):
        raise TypeError(f"yaml_config['{parent}'] must be a dict")

    renamed = {}

    for key, value in block.items():
        key_str = str(key)
        if key_str in ("include_h", "include_cpp"):
            renamed[key_str] = value
            continue

        new_key = key_str if key_str.endswith(suffix) else key_str + suffix
        if new_key in renamed:
            raise KeyError(
                f"Suffix operation would overwrite existing key '{new_key}'. "
                f"Original key: '{key_str}'"
            )
        renamed[new_key] = value

    yaml_config[parent] = renamed
    return yaml_config


def ensure_value_suffix(yaml_config: dict, parent: str, field: str, suffix: str):
    block = yaml_config.get(parent)
    if block is None:
        raise KeyError(f"Parent key '{parent}' not found")

    if not isinstance(block, dict):
        raise TypeError(f"yaml_config['{parent}'] must be a dict")

    for key, entry in block.items():
        if key in ("include_h", "include_cpp"):
            continue
        if field not in entry:
            raise KeyError(
                f"Entry '{key}' in '{parent}' does not contain required field '{field}'"
            )

        if isinstance(entry[field], list):
            for i, v in enumerate(entry[field]):
                val = str(v)
                if not val.endswith(suffix):
                    entry[field][i] = val + suffix
        else:
            val = str(entry[field])
            if not val.endswith(suffix):
                entry[field] = val + suffix

    return yaml_config
