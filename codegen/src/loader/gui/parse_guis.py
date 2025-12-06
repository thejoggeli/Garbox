from common.util import print_json
from common.str_filters import to_camel_case
from pathlib import Path
from loader.gui.preprocess_xmls import preprocess_xml_text
from loader.gui.convert_xml_to_dict import xml_to_dict
from loader.gui.postprocess_dict import postprocess_dict

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
    converted_dicts = {}
    for name, xml_text in preprocessed_texts.items():
        # convert XML to dict
        converted_dicts[name] = xml_to_dict(xml_text)

    # postprocessing dicts
    postprocessed_dicts = {}
    for name, converted_dict in converted_dicts.items():
        # convert XML to dict
        postprocessed_dicts[name] = postprocess_dict(converted_dict)

    return postprocessed_dicts

