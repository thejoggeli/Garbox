from common.util import print_json
from common.str_filters import to_camel_case
from pathlib import Path
from loader.gui.preprocess_xmls import preprocess_xml_text
from loader.gui.convert_xml_to_dict import xml_to_dict
from loader.gui.init_cmds import build_initializer_commands

def parse_guis(xml_texts: str, save_preprocessed_to_dir: Path):

    # preprocessing
    preprocessed_texts = {}
    for name, xml_text in xml_texts.items():

        # preprocess XML text
        xml_text_preprocessed = preprocess_xml_text(xml_text)
        preprocessed_texts[name] = xml_text_preprocessed

        # save preprocessed XML for inspection
        save_preprocessed_to_dir.mkdir(parents=True, exist_ok=True)
        out_path = save_preprocessed_to_dir / f"{name}.xml"
        with open(out_path, "w") as file:
            file.write(xml_text_preprocessed)

    # convert XMLs to dicts
    gui_dicts = {}
    for name, xml_text in preprocessed_texts.items():
        # convert XML to dict
        gui_dicts[name] = xml_to_dict(xml_text)

    # build initializer commands
    for name, gui_data in gui_dicts.items():
        # convert XML to dict
        gui_data["init_cmds"] = build_initializer_commands(gui_data)

    return gui_dicts

