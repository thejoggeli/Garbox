INT_TYPES = {
    "int8_t", "uint8_t",
    "int16_t", "uint16_t",
    "int32_t", "uint32_t",
    "int64_t", "uint64_t",
    "int", "signed int",
    "unsigned", "unsigned int",
    "long", "signed long",
    "unsigned long",
    "short", "signed short",
    "unsigned short",
    "long long", "signed long long",
    "unsigned long long",
}

TRUE_SET  = {"1", "yes", "true",  "on",  "enable",  "enabled"}
FALSE_SET = {"0", "no",  "false", "off", "disable", "disabled", "none", "null", "undefined"}


def preprocess_type(type_name: str):
    result = type_name.lower().strip()
    if(result == "str"):
        result = "string"
    elif(result == "boolean"):
        result = "bool"
    return result


def get_null_value(type_name: str):
    type_name = preprocess_type(type_name)
    if(type_name == "string"):
        return ""
    if(type_name == "bool"):
        return False
    if(type_name == "float"):
        return 0.0
    if(is_int_type(type_name)):
        return 0
    return None
 

def is_int_type(type_name: str):
    type_name = type_name.lower().strip()
    return type_name in INT_TYPES


def parse_bool(value):
    """
    Strict boolean parser
    """

    # Always convert to string (except None, which becomes "none")
    if value is None:
        v = "none"
    else:
        v = str(value)

    # Normalize
    v = v.strip().lower()

    if v in TRUE_SET:
        return "true"
    if v in FALSE_SET:
        return "false"

    raise ValueError(f"Invalid boolean literal: {value!r}")


def parse_int_for_type(value, type_name):
    """
    Strict integer parsing for:
        - int8_t,  uint8_t
        - int16_t, uint16_t
        - int32_t, uint32_t
        - int64_t, uint64_t
        - int, unsigned int
        - long, unsigned long
        - short, unsigned short
        - long long, unsigned long long

    Raises ValueError on invalid or out-of-range values.
    """

    type_name = preprocess_type(type_name)

    # Fixed-width integer ranges
    FIXED_INT_RANGES = {
        "int8_t":   (-128, 127),
        "uint8_t":  (0, 255),
        "int16_t":  (-32768, 32767),
        "uint16_t": (0, 65535),
        "int32_t":  (-2147483648, 2147483647),
        "uint32_t": (0, 4294967295),
        "int64_t":  (-9223372036854775808, 9223372036854775807),
        "uint64_t": (0, 18446744073709551615),
    }

    if isinstance(value, str):
        value = value.replace("'", "")

    # Try converting to int
    try:
        n = int(value)
    except Exception:
        raise ValueError(f"Invalid integer literal for {type_name}: {value!r}")

    # Check fixed-width ranges
    if type_name in FIXED_INT_RANGES:
        lo, hi = FIXED_INT_RANGES[type_name]
        if not (lo <= n <= hi):
            raise ValueError(
                f"{type_name} out of range: {n}, must be in [{lo}, {hi}]"
            )
        return n

    # Generic signed integer types
    SIGNED_GENERIC = {
        "int", "signed int",
        "long", "signed long",
        "long long", "signed long long",
        "short", "signed short",
    }

    # Generic unsigned integer types
    UNSIGNED_GENERIC = {
        "unsigned", "unsigned int",
        "unsigned long",
        "unsigned long long",
        "unsigned short",
    }

    # Signed -> accept any Python int
    if type_name in SIGNED_GENERIC:
        return n

    # Unsigned -> must be >= 0
    if type_name in UNSIGNED_GENERIC:
        if n < 0:
            raise ValueError(f"{type_name} cannot be negative: {n}")
        return n

    raise ValueError(f"Unsupported integer type: {type_name}")


def render_value(value, type_name):
    """
    Convert a Python value + string field type into a strictly typed code-generation literal.

    Rules:
      - None                -> None (no default)
      - bool                -> "true" / "false" (strict parsing via parse_bool)
      - float               -> <number>f        (must parse as float)
      - int types           -> integer literal  (validated ranges via parse_int_for_type)
      - string              -> quoted "value"
      - enums (identifiers) -> returned as-is   (must be a non-empty string)

    All conversions are strict:
      - any invalid literal raises ValueError
      - no silent fallbacks or coercions
    """

    type_name = preprocess_type(type_name)

    if value is None:
        return None

    # bool
    if type_name == "bool":
        return parse_bool(value)

    # float
    if type_name == "float":
        try:
            num = float(value)
        except Exception:
            raise ValueError(f"Invalid float literal: {value!r}")
        # 1f or 1.5f
        if num.is_integer():
            return f"{int(num)}.0f"
        return f"{num}f"

    # integers
    if type_name in INT_TYPES:
        return parse_int_for_type(value, type_name)

    # string
    if type_name == "string":
        if not isinstance(value, str):
            raise ValueError(f"String literal required, got {value!r}")
        return f"\"{value}\""

    # enum (anything not primitive)
    if type_name not in ("float", "bool", "string") and type_name not in INT_TYPES:
        if not isinstance(value, str) or not value:
            raise ValueError(f"Invalid enum literal {value!r} for type {type_name}")
        return value

    return value