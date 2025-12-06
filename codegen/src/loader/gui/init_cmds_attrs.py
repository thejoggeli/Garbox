def split_attr(val):
    split = val.replace(",", " ")
    split = val.split(" ")
    return split


def process_attrs(attrs):

    for attr_type, attr_data in attrs.items():

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


