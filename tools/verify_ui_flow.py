#!/usr/bin/env python3
"""Exercise the complete Garbox UI flow and optionally capture every screen."""

from __future__ import annotations

import argparse
from pathlib import Path

from ui_simulator import HEIGHT, PROGRAM, WIDTH, Simulator


def short_press(simulator: Simulator) -> None:
    simulator.command("button press")
    simulator.command("button release")


def long_press(simulator: Simulator) -> None:
    simulator.command("button press")
    simulator.command("button long")
    simulator.command("button release")


def capture(
    simulator: Simulator,
    expected_screen: str,
    name: str,
    output_dir: Path | None,
) -> dict[str, str]:
    rgb, metadata = simulator.frame()
    actual_screen = metadata["screen"]
    if actual_screen != expected_screen:
        raise AssertionError(f"expected {expected_screen}, got {actual_screen}")
    if len(rgb) != WIDTH * HEIGHT * 3:
        raise AssertionError(f"invalid frame length: {len(rgb)}")
    if output_dir is not None:
        output_dir.mkdir(parents=True, exist_ok=True)
        destination = output_dir / f"{name}.ppm"
        destination.write_bytes(f"P6\n{WIDTH} {HEIGHT}\n255\n".encode("ascii") + rgb)
        print(f"captured {actual_screen}: {destination}")
    else:
        print(f"verified {actual_screen}")
    return metadata


def verify(output_dir: Path | None) -> None:
    simulator = Simulator(PROGRAM)
    try:
        initial = capture(simulator, "Simple", "01-home-off", output_dir)

        simulator.command("encoder 5")
        short_press(simulator)
        simulator.command("step 2")
        active = capture(simulator, "Simple", "02-home-running", output_dir)
        if active["target"] != "32.500" or active["power"] != "1":
            raise AssertionError(f"home controls did not update application state: {active}")

        long_press(simulator)
        capture(simulator, "Main", "03-trends-controls", output_dir)

        # Binary controls toggle immediately and remain in browse mode.
        short_press(simulator)
        power_off = capture(simulator, "Main", "03a-power-off", output_dir)
        if power_off["power"] != "0":
            raise AssertionError(f"Main power control failed: {power_off}")

        short_press(simulator)
        power_on = capture(simulator, "Main", "03b-power-on", output_dir)
        if power_on["power"] != "1":
            raise AssertionError(f"Main power control failed: {power_on}")

        # Numeric controls retain explicit press/edit/press behavior.
        simulator.command("encoder 1")
        short_press(simulator)
        capture(simulator, "Main", "03c-target-edit", output_dir)
        simulator.command("encoder 5")
        short_press(simulator)
        target_up = capture(simulator, "Main", "03d-target-up", output_dir)
        if target_up["target"] != "33.000":
            raise AssertionError(f"Main target control failed: {target_up}")
        short_press(simulator)
        simulator.command("encoder -5")
        short_press(simulator)

        # Fan mode is binary, while manual speed remains a numeric editor.
        simulator.command("encoder 1")
        short_press(simulator)
        simulator.command("encoder 1")
        short_press(simulator)
        simulator.command("encoder 7")
        short_press(simulator)
        simulator.command("step 1")
        manual_fan = capture(simulator, "Main", "03e-manual-fan", output_dir)
        if (manual_fan["fan_auto"], manual_fan["fan_manual"], manual_fan["fan_output"]) != (
            "0",
            "0.350",
            "0.350",
        ):
            raise AssertionError(f"Main manual fan control failed: {manual_fan}")

        # Fan speed is row three; Diagnostics is three rows clockwise.
        simulator.command("encoder 3")
        short_press(simulator)
        capture(simulator, "Debug", "04-diagnostics", output_dir)

        short_press(simulator)
        capture(simulator, "EventLog", "05-event-log", output_dir)

        short_press(simulator)
        capture(simulator, "StateLog", "06-state-log", output_dir)

        short_press(simulator)
        capture(simulator, "Debug", "07-diagnostics-loop", output_dir)

        long_press(simulator)
        capture(simulator, "Main", "08-main-return", output_dir)

        long_press(simulator)
        final = capture(simulator, "Simple", "09-home-return", output_dir)
        if final["target"] != "32.500" or final["power"] != "1":
            raise AssertionError(f"state was lost while navigating: {final}")
    finally:
        simulator.close()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--capture-dir",
        type=Path,
        help="write 320x240 PPM captures to this directory",
    )
    args = parser.parse_args()
    if not PROGRAM.is_file():
        parser.error(f"simulator executable not found: {PROGRAM}; build ui-simulator first")
    verify(args.capture_dir)
    print("UI flow verified")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
