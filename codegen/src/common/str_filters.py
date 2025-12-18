import re

def to_camel_case(name: str, delim: str | None = None) -> str:
    # Case 1: delimiter-based conversion
    if delim and delim in name:
        parts = name.split(delim)
        first = parts[0].lower()
        rest = [p[:1].upper() + p[1:] for p in parts[1:]]
        return first + "".join(rest)

    # Case 2: already camelCase or PascalCase
    # Only change the first character if it's uppercase
    if name and name[0].isupper():
        return name[0].lower() + name[1:]
    return name

def to_snake_case(name: str, delim: str | None = None) -> str:
    # Case 1: delimiter-based conversion
    if delim and delim in name:
        parts = name.split(delim)
        return "_".join(p.lower() for p in parts if p)

    # Case 2: camelCase or PascalCase → snake_case
    if not name:
        return name

    # Insert underscore before capitals (except at start), then lowercase
    snake = re.sub(r'(?<!^)(?=[A-Z])', '_', name)
    return snake.lower()

def upper_first(s):
    if not isinstance(s, str) or not s:
        return s
    return s[0].upper() + s[1:]

def lower_first(s):
    if not isinstance(s, str) or not s:
        return s
    return s[0].lower() + s[1:]

def ljust(s:str, width:int, fillchar:str=" "):
    return s.ljust(width, fillchar)

def rjust(s:str, width:int, fillchar:str=" "):
    return s.rjust(width, fillchar)

def ensure_suffix(value: str, suffix: str):
    if not value.endswith(suffix):
        return value + suffix
    return value
