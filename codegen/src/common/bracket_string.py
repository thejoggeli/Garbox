import re

def is_bracket_string(string):
    return "(" in string and string.endswith(")")


def parse_bracket_string(string):
    """
    Convert a single bracket-style definition into structured components.

    Example:
        Input:
            "FanStatus(targetSpeed, epsilon=1e-3, default=0.0)"

        Output:
            {
                "name": "FanStatus",
                "args": ["targetSpeed"],
                "kwargs": {"epsilon": 1e-3, "default": 0.0}
            }

    Rules:
        - Everything before '(' is the name.
        - Comma-separated tokens inside parentheses.
        - Tokens without '=' are positional args.
        - Tokens with '=' are parsed as keyword args.
        - Keyword values are parsed using safe eval when possible.
        - Empty string inside parentheses => no args.
    """

    if not isinstance(string, str):
        raise TypeError(f"Expected string, got {type(string).__name__}")

    string = string.strip()

    # Match: Name(argstuff)
    m = re.match(r"^([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*(.*?)\s*\)$", string)
    if not m:
        raise ValueError(f"Invalid bracket expression: {string!r}")

    name = m.group(1)
    inside = m.group(2).strip()

    # No arguments
    if not inside:
        return {"name": name, "args": [], "kwargs": {}}

    # Split on commas at top-level
    parts = [p.strip() for p in inside.split(",") if p.strip()]

    args = []
    kwargs = {}

    for token in parts:
        if "=" in token:
            # keyword argument
            key, val = token.split("=", 1)
            key = key.strip()
            val = val.strip()

            # try safe evaluation of literal
            try:
                parsed_val = eval(val, {"__builtins__": {}})
            except Exception:
                # keep as raw string if not evaluable
                parsed_val = val

            kwargs[key] = parsed_val
        else:
            # positional arg (field name)
            args.append(token)

    return {"name": name, "args": args, "kwargs": kwargs}
