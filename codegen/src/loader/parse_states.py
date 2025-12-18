from common.parse_type import render_value
from common.util import ensure_list
from common.str_filters import ensure_suffix, to_snake_case

def parse_states(config: dict):

    states = config["states"]

    for state_name, state_data in states.items():

        if(state_data is None):
            states[state_name] = {"meta": {}, "fields": {}}
            continue
    
        state_meta = state_data.get("meta", {})
        state_fields = state_data.get("fields", [])
        if state_fields is None:
            state_fields = []

        parsed_fields = {}

        for state_field in state_fields:

            field_name = state_field["name"]
            field_type = state_field["type"]
            default_value = state_field.get("default", None)

            parsed_fields[field_name] = {
                "name": field_name,
                "type": field_type,
                "default": render_value(default_value, field_type)
            }

        include = ensure_list(state_data.get("include", []))

        states[state_name] = {
            "name": state_name,
            "name_with_suffix": ensure_suffix(state_name, "State"),
            "include": include,
            "meta": state_meta,
            "fields": parsed_fields,
        }

