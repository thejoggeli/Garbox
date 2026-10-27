from __future__ import annotations

import contextlib
import io
import shutil
import sys
import tempfile
import unittest
from pathlib import Path


REPOSITORY_ROOT = Path(__file__).resolve().parents[2]
CODEGEN_SOURCE = REPOSITORY_ROOT / "codegen/src"
sys.path.insert(0, str(CODEGEN_SOURCE))

from loader.gui.convert_xml_to_dict import xml_to_dict
from loader.gui.init_cmds_fns import eval_initializer
from loader.gui.init_cmds_render import split_attr
from loader.loader import merge_yaml
from loader.parse_application import process_tick_phases
from loader.parse_events import parse_events
from loader.parse_states import parse_states
from loader.validate_config import validate_raw_config
from common.parse_type import render_value
from main import generate_project


def make_minimal_config():
    return {
        "events": {"Known": None},
        "states": {"Status": None},
        "controllers": {"Input": {}},
        "behaviours": {"Run": {}},
        "screens": {"Home": {}},
        "application": {
            "app_name": "Fixture",
            "initial_behaviour": "RunBehaviour",
            "initial_screen": "HomeScreen",
            "event_pool_size_bytes": 32,
            "event_queue_length": 2,
            "tick_period_millis": 10,
            "max_dispatch_recursion_depth": 2,
            "tick_phases": [{"name": "Logic"}],
        },
        "hardware": {"ledc": {"instances": {"timers": {}, "channels": {}}}},
    }


class LoaderValidationTests(unittest.TestCase):
    def test_duplicate_definitions_across_files_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "events.Sample"):
            merge_yaml(
                {"events": {"Sample": {"fields": []}}},
                {"events": {"Sample": {"fields": []}}},
            )

    def test_unknown_component_reference_is_rejected(self):
        config = make_minimal_config()
        config["controllers"]["Input"]["send_events"] = ["Missing"]
        with self.assertRaisesRegex(ValueError, "unknown value 'Missing'"):
            validate_raw_config(config)

    def test_output_names_and_cpp_keywords_are_rejected_before_rendering(self):
        config = make_minimal_config()
        config["events"] = {"Pulse": None, "PulseEvent": None}
        with self.assertRaisesRegex(ValueError, "collision 'PulseEvent'"):
            validate_raw_config(config)

        config = make_minimal_config()
        config["states"] = {"class": None}
        with self.assertRaisesRegex(ValueError, "reserved C\\+\\+ keyword"):
            validate_raw_config(config)

    def test_component_subdirectories_cannot_escape_staging_tree(self):
        config = make_minimal_config()
        config["controllers"]["Input"]["subdir"] = "../../outside"
        with self.assertRaisesRegex(ValueError, "stay inside"):
            validate_raw_config(config)

        config["controllers"]["Input"]["subdir"] = "/tmp/outside"
        with self.assertRaisesRegex(ValueError, "stay inside"):
            validate_raw_config(config)

    def test_insert_all_must_be_the_only_supported_reference(self):
        config = make_minimal_config()
        config["controllers"]["Input"]["receive_events"] = ["INSERT_ALL", "Known"]
        with self.assertRaisesRegex(ValueError, "must be the only entry"):
            validate_raw_config(config)

        config = make_minimal_config()
        config["controllers"]["Input"]["send_events"] = ["INSERT_ALL"]
        with self.assertRaisesRegex(ValueError, "unknown value 'INSERT_ALL'"):
            validate_raw_config(config)

    def test_implicit_tick_orders_skip_explicit_values(self):
        config = {
            "application": {
                "tick_phases": [
                    {"name": "First", "order": 0},
                    {"name": "Second"},
                    {"name": "Third", "order": 2},
                    {"name": "Fourth"},
                ]
            },
            "controllers": {},
            "behaviours": {},
            "screens": {},
        }
        process_tick_phases(config)
        self.assertEqual(
            [phase["order"] for phase in config["application"]["tick_phases"]],
            [0, 1, 2, 3],
        )

    def test_empty_event_and_state_definitions_are_fully_initialized(self):
        config = {"events": {"Ping": None}, "states": {"Ready": None}}
        parse_events(config)
        parse_states(config)
        self.assertEqual(config["events"]["Ping"]["name_with_suffix"], "PingEvent")
        self.assertEqual(config["states"]["Ready"]["name_with_suffix"], "ReadyState")
        self.assertEqual(config["states"]["Ready"]["readers"]["components"], {})

    def test_cpp_literals_reject_nonfinite_values_and_escape_strings(self):
        for value in ("nan", "inf", "-inf"):
            with self.assertRaisesRegex(ValueError, "must be finite"):
                render_value(value, "float")
        self.assertEqual(render_value('say "hello"\n', "string"), '"say \\"hello\\"\\n"')
        with self.assertRaisesRegex(ValueError, "Invalid enum literal"):
            render_value("Value; injected()", "SomeEnum")


class GuiCompilerTests(unittest.TestCase):
    def test_comma_and_repeated_whitespace_are_normalized(self):
        self.assertEqual(split_attr("left,  top   right"), ["left", "top", "right"])

    def test_dormant_initializer_mappings_target_correct_api(self):
        self.assertEqual(
            eval_initializer("object", "transform-skew", "10deg, 20deg"),
            ["setTransformSkew(100, 200)"],
        )
        self.assertEqual(
            eval_initializer("object", "border-post", "true"),
            ["setBorderPost(true)"],
        )

    def test_duplicate_component_definition_is_rejected(self):
        xml = """
        <gui><components>
          <Card><body /></Card>
          <Card><body /></Card>
        </components><root /></gui>
        """
        with self.assertRaisesRegex(ValueError, "duplicate component"):
            xml_to_dict("Fixture", xml)

    def test_names_that_collide_after_normalization_are_rejected(self):
        xml = """
        <gui><root>
          <label name="same-name" />
          <label name="sameName" />
        </root></gui>
        """
        with self.assertRaisesRegex(ValueError, "after normalization"):
            xml_to_dict("Fixture", xml)

    def test_duplicate_resource_definition_is_rejected(self):
        xml = """
        <gui><resources>
          <image name="icon" path="one.png" />
          <image name="icon" path="two.png" />
        </resources><root /></gui>
        """
        with self.assertRaisesRegex(ValueError, "duplicate resource"):
            xml_to_dict("Fixture", xml)

    def test_xml_cpp_names_and_document_types_are_rejected(self):
        invalid_name = """
        <gui><root><label name="not.valid" /></root></gui>
        """
        with self.assertRaisesRegex(ValueError, "GUI object name"):
            xml_to_dict("Fixture", invalid_name)

        invalid_resource = """
        <gui><resources><image name="bad.name" path="icon.png" /></resources><root /></gui>
        """
        with self.assertRaisesRegex(ValueError, "resource name"):
            xml_to_dict("Fixture", invalid_resource)

        entity_document = """<!DOCTYPE gui [<!ENTITY local SYSTEM "file:///etc/passwd">]>
        <gui><root><label text="&local;" /></root></gui>
        """
        with self.assertRaisesRegex(ValueError, "document types"):
            xml_to_dict("Fixture", entity_document)


class TransactionalGenerationTests(unittest.TestCase):
    def make_fixture(self, destination: Path):
        for relative in (
            Path("codegen/config"),
            Path("codegen/templates"),
            Path("res"),
            Path("src/app"),
            Path("src/shared"),
        ):
            shutil.copytree(REPOSITORY_ROOT / relative, destination / relative)

    def snapshot(self, root: Path):
        result = {}
        for relative in (Path("src/generated"), Path("src/bin"), Path("src/shared"), Path("src/app")):
            for path in sorted((root / relative).rglob("*")):
                if path.is_file():
                    result[path.relative_to(root)] = path.read_bytes()
        return result

    def test_failed_render_leaves_all_outputs_unchanged(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_fixture(root)
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertTrue(generate_project(root))
            before = self.snapshot(root)

            broken_template = root / "codegen/templates/config/Config.h.j2"
            broken_template.write_text(broken_template.read_text() + "\n{% if\n")
            with contextlib.redirect_stdout(io.StringIO()):
                with self.assertRaises(Exception):
                    generate_project(root)

            self.assertEqual(self.snapshot(root), before)

    def test_check_mode_detects_stale_output_without_modifying_it(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_fixture(root)
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertTrue(generate_project(root))
                self.assertTrue(generate_project(root, check=True))

            generated = root / "src/generated/config/GeneratedConfig.h"
            generated.write_text(generated.read_text() + "// stale\n")
            stale_contents = generated.read_bytes()
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertFalse(generate_project(root, check=True))
            self.assertEqual(generated.read_bytes(), stale_contents)


if __name__ == "__main__":
    unittest.main()
