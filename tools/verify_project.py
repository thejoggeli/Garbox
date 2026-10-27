#!/usr/bin/env python3
"""Run the reproducible Garbox software verification sequence."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def platformio_executable() -> str:
    executable = shutil.which("pio")
    if executable:
        return executable
    fallback = Path.home() / ".platformio/penv/bin/pio"
    if fallback.is_file():
        return str(fallback)
    raise FileNotFoundError(
        "PlatformIO was not found; install it or use the PlatformIO IDE extension"
    )


def run(label: str, command: list[str]) -> None:
    print(f"\n=== {label} ===", flush=True)
    subprocess.run(command, cwd=ROOT, check=True)


def parse_args(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--skip-ui", action="store_true", help="skip UI build and flow verification")
    parser.add_argument("--skip-target", action="store_true", help="skip ESP32 firmware compilation")
    return parser.parse_args(argv)


def main(argv=None) -> int:
    args = parse_args(argv)
    run(
        "generated output check",
        [sys.executable, "codegen/src/main.py", "--check", "--quiet"],
    )
    run(
        "generator tests",
        [sys.executable, "-W", "error::ResourceWarning", "-m", "unittest", "discover", "-s", "codegen/tests", "-v"],
    )

    pio = platformio_executable()
    run("native tests", [pio, "test", "-e", "native"])
    if not args.skip_ui:
        run("desktop LVGL build", [pio, "run", "-e", "ui-simulator"])
        run("desktop UI flow", [sys.executable, "tools/verify_ui_flow.py"])
    if not args.skip_target:
        run("ESP32 firmware build", [pio, "run", "-e", "esp32-s3-devkitc-1"])

    print("\nAll requested software verification passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
