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


def upper_first(s):
    if not isinstance(s, str) or not s:
        return s
    return s[0].upper() + s[1:]


def lower_first(s):
    if not isinstance(s, str) or not s:
        return s
    return s[0].lower() + s[1:]