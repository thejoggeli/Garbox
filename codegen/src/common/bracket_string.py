import re

class BracketString:

    name: str = None
    args: list = []
    kwargs: dict = {}

    def __init__(self, raw_string):
        self.raw_string = raw_string
        self.parse_bracket_string()

    
    def require_get_arg(self, index):
        if(index < 0 or index >= len(self.args)):
            raise IndexError("required index not in {self.name}.args: index={index}")
        return self.args[index]


    def require_get_kwarg(self, key):
        if(key not in self.kwargs):
            raise KeyError("required key not in {self.name}.kwargs: key={key}")
        return self.kwargs[key]
    
    
    def try_get_arg(self, index, default = None):
        if(index < 0 or index >= len(self.args)):
            return default
        return self.args[index]
    

    def try_get_kwarg(self, key, default = None):
        return self.kwargs.get(key, default)


    def has_kwarg(self, key):
        return key in self.kwargs


    def parse_bracket_string(self):
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

        if not isinstance(self.raw_string, str):
            raise TypeError(f"Expected string, got {type(self.raw_string).__name__}")

        string = self.raw_string.strip()

        # Match: Name(argstuff)
        m = re.match(r"^([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*(.*?)\s*\)$", string)
        if not m:
            raise ValueError(f"Invalid bracket expression: {string!r}")

        self.name = m.group(1)
        inside = m.group(2).strip()

        # No arguments
        if not inside:
            return

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

        self.args = args
        self.kwargs = kwargs

