from common.str_filters import upper_first
from common.parse_type import render_value, get_null_value

def parse_datastore(config: dict):

    aliases = {}

    lookup = {
        "uint8_t": "StoredUint8",
        "uint16_t": "StoredUint16",
        "uint32_t": "StoredUint32",
        "uint64_t": "StoredUint64",
        "int8_t": "StoredInt8",
        "int16_t": "StoredInt16",
        "int32_t": "StoredInt32",
        "int64_t": "StoredInt64",
        "bool": "StoredBool",
        "float": "StoredFloat",
        "double": "StoredDouble",
    }

    for field in config["datastore"]["fields"]:

        type_name = field["type"]
        if type_name in lookup:
            alias_name = lookup[type_name]
        else:
            alias_name = "Stored" + upper_first(type_name)
        
        # aliases list
        aliases[alias_name] = {
            "name": alias_name,
            "type": type_name,
        }

        # add stuff to datastore field
        field["type_name"] = type_name
        field["type_alias"] = alias_name

    config["datastore"]["type_aliases"] = aliases