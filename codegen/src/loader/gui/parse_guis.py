from common.util import print_json
from common.str_filters import to_camel_case
from pathlib import Path
from loader.gui.preprocess_xmls import preprocess_xml_text
from loader.gui.convert_xml_to_dict import xml_to_dict
from loader.gui.resolve_components import resolve_components
from loader.gui.init_cmds import build_initializer_commands
from loader.gui.resources import build_resources

def parse_guis(xml_texts: str, output_dir: Path):

    # preprocessing
    preprocessed_texts = {}
    for name, xml_text in xml_texts.items():
        print(f"{name}: preprocessing raw xml")

        # preprocess XML text
        xml_text_preprocessed = preprocess_xml_text(xml_text)
        preprocessed_texts[name] = xml_text_preprocessed

        # save preprocessed XML for inspection
        output_dir.mkdir(parents=True, exist_ok=True)
        out_path = output_dir / f"{name}.xml"
        with open(out_path, "w") as file:
            file.write(xml_text_preprocessed)

    # convert XMLs to dicts
    gui_dicts = {}
    for name, xml_text in preprocessed_texts.items():
        print(f"{name}: converting raw xml to dicts")
        gui_dicts[name] = xml_to_dict(name, xml_text)

    # resolve components => builds flat objects registry
    for name, gui_data in gui_dicts.items():
        print(f"{name}: resolving components")
        gui_data["objects_flat"] = resolve_components(gui_data)

    # process resources
    for name, gui_data in gui_dicts.items():
        print(f"{name}: building resources")
        build_resources(name, gui_data)

    # build initializer commands
    for name, gui_data in gui_dicts.items():
        print(f"{name}: building init commands")
        gui_data["init_cmds"] = build_initializer_commands(gui_data)

    return gui_dicts

