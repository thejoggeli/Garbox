def split_attr(val):
    split = val.replace(",", " ")
    split = val.split(" ")
    return split

    if isinstance(v, str) and v.lower() == "cover":
        return 255

def parse_bg_opacity(v):
    
    # case: literal string "cover"
    if isinstance(v, str) and v.strip().lower() == "cover":
        return 255
    
    # case: "NN%" string
    if isinstance(v, str) and v.endswith("%"):
        try:
            pct = float(v[:-1])
        except ValueError:
            raise ValueError(f"Invalid opacity percent: {v}")

        value = int(round((pct / 100.0) * 255))
        return max(0, min(255, value))

    # case: int or convertible-to-int
    try:
        num = int(v)
    except Exception:
        raise ValueError(f"Invalid opacity value: {v}")

    return max(0, min(255, num))


def process_attrs(attrs):

    for attr_type, attr_data in attrs.items():

        if attr_type in {"bg-opacity", "text-outline-opa", "text-opa"}:
            attr_data["value"] = parse_bg_opacity(attr_data["value"])

        # attribute 'pad'
        if attr_type == "pad":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = attr_data = [split[0], split[0], split[0], split[0]]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[0], split[1], split[1]]
            elif len(split) == 4:
                attr_data["value"] = [split[0], split[1], split[2], split[3]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected 1 or 2 or 4 integers, got '{attr_data['value']}'")
        
        # attribute 'pad-x'
        elif attr_type == "pad-x" or attr_type == "pad-y":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = [split[0], split[0]]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[1]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected 1 or 2 integers, got '{attr_data['value']}'")

        # align
        elif attr_type == "align":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = [split[0], 0, 0]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[1], split[1]]
            elif len(split) == 3:
                attr_data["value"] = [split[0], split[1], split[2]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected str and up to 2 integers, got '{attr_data['value']}'")

        # flex_align
        elif attr_type == "flex-align":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = [split[0], "start", "start"]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[1], "start"]
            elif len(split) == 3:
                attr_data["value"] = [split[0], split[1], split[2]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected 1 or 2 or 3 values, got '{attr_data['value']}'")