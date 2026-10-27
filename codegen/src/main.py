#!/usr/bin/env python3
"""Generate Garbox sources safely and reproducibly.

All rendering happens in a staging tree. The real source tree is touched only
after loading, validation, section replacement, templates, and image conversion
have completed successfully.
"""

from __future__ import annotations

import argparse
import contextlib
import difflib
import filecmp
import io
import os
import shutil
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path

from common.context import Context
from common.util import print_section
from generate.gen_components import generate_components
from generate.gen_config import generate_config
from generate.gen_guis import generate_guis
from generate.gen_hardware import generate_hardware
from generate.gen_runtime import generate_runtime
from generate.gen_shared import generate_shared
from generate.gen_states import generate_states
from loader.loader import Loader


OWNED_DIRECTORIES = (
    Path("src/generated"),
    Path("src/bin"),
)
OVERLAY_DIRECTORIES = (
    Path("src/shared"),
    Path("src/app"),
)
DIAGNOSTIC_DIRECTORY = Path("codegen/output")


@dataclass(frozen=True)
class Change:
    kind: str
    relative_path: Path


def make_context(source_root: Path, destination_root: Path) -> Context:
    source_root = source_root.resolve()
    destination_root = destination_root.resolve()
    return Context(
        root_dir=source_root,
        config_dir=source_root / "codegen/config",
        output_dir=destination_root / "codegen/output",
        stubs_dir=destination_root / "codegen/output/stubs",
        templates_dir=source_root / "codegen/templates",
        res_dir=source_root / "res",
        app_dir=destination_root / "src/app",
        gen_dir=destination_root / "src/generated",
        bin_dir=destination_root / "src/bin",
        shared_dir=destination_root / "src/shared",
    )


def prepare_staging_tree(source_root: Path, staging_root: Path) -> None:
    """Copy only handwritten trees that generation may update in place."""
    for relative in OVERLAY_DIRECTORIES:
        source = source_root / relative
        destination = staging_root / relative
        if source.exists():
            shutil.copytree(source, destination)


def render_to_staging(source_root: Path, staging_root: Path) -> None:
    context = make_context(source_root, staging_root)
    print_section("generator staging paths")
    context.print()

    print_section("loading and validating configuration")
    loader = Loader(context)
    loader.preload_all()

    print_section("saving processed configuration")
    loader.save_config(context.output_dir / "parsed")

    generators = (
        ("hardware", generate_hardware),
        ("shared files", generate_shared),
        ("states", generate_states),
        ("components", generate_components),
        ("runtime", generate_runtime),
        ("configuration", generate_config),
        ("GUIs and image assets", generate_guis),
    )
    for label, generator in generators:
        print_section(f"generating {label}")
        generator(context, loader)

    if not context.gen_dir.is_dir() or not any(context.gen_dir.rglob("*")):
        raise RuntimeError("generation produced no files in src/generated")


def _files_under(root: Path, relative: Path) -> dict[Path, Path]:
    directory = root / relative
    if not directory.exists():
        return {}
    return {
        path.relative_to(root): path
        for path in sorted(directory.rglob("*"))
        if path.is_file()
    }


def collect_changes(source_root: Path, staging_root: Path) -> list[Change]:
    changes = []
    for relative in OWNED_DIRECTORIES:
        current = _files_under(source_root, relative)
        staged = _files_under(staging_root, relative)
        for path in sorted(set(current) | set(staged)):
            if path not in current:
                changes.append(Change("add", path))
            elif path not in staged:
                changes.append(Change("delete", path))
            elif not filecmp.cmp(current[path], staged[path], shallow=False):
                changes.append(Change("modify", path))

    # Overlay trees begin as exact copies of the source. Only generated
    # sections and newly-created component stubs can differ.
    for relative in OVERLAY_DIRECTORIES:
        current = _files_under(source_root, relative)
        staged = _files_under(staging_root, relative)
        for path in sorted(staged):
            if path not in current:
                changes.append(Change("add", path))
            elif not filecmp.cmp(current[path], staged[path], shallow=False):
                changes.append(Change("modify", path))
    return changes


def _read_text(path: Path) -> list[str] | None:
    try:
        if path.stat().st_size > 128_000:
            return None
        return path.read_text().splitlines(keepends=True)
    except (OSError, UnicodeDecodeError):
        return None


def print_changes(changes: list[Change], source_root: Path, staging_root: Path) -> None:
    for change in changes:
        print(f"{change.kind:>6}  {change.relative_path}")
        if change.kind != "modify":
            continue
        before = _read_text(source_root / change.relative_path)
        after = _read_text(staging_root / change.relative_path)
        if before is None or after is None:
            continue
        diff = difflib.unified_diff(
            before,
            after,
            fromfile=f"a/{change.relative_path}",
            tofile=f"b/{change.relative_path}",
            n=2,
        )
        for line in diff:
            print(line, end="")


def apply_staged_output(source_root: Path, staging_root: Path, changes: list[Change]) -> None:
    """Apply a fully rendered tree with rollback on any filesystem failure."""
    backup_root = staging_root / ".backup"
    backup_root.mkdir()
    replaced_directories: list[tuple[Path, Path | None]] = []
    replaced_files: list[tuple[Path, Path | None]] = []
    temporary_files: list[Path] = []

    try:
        changed_owned_directories = [
            relative
            for relative in OWNED_DIRECTORIES
            if any(change.relative_path.is_relative_to(relative) for change in changes)
        ]
        for relative in (*changed_owned_directories, DIAGNOSTIC_DIRECTORY):
            staged = staging_root / relative
            destination = source_root / relative
            backup = backup_root / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            backup.parent.mkdir(parents=True, exist_ok=True)
            existing_backup = None
            if destination.exists():
                destination.replace(backup)
                existing_backup = backup
            # Record the operation before installing the replacement so even
            # a failure in the second rename restores the original directory.
            replaced_directories.append((destination, existing_backup))
            staged.replace(destination)

        for change in changes:
            if not any(change.relative_path.is_relative_to(root) for root in OVERLAY_DIRECTORIES):
                continue
            source = staging_root / change.relative_path
            destination = source_root / change.relative_path
            backup = backup_root / change.relative_path
            destination.parent.mkdir(parents=True, exist_ok=True)
            backup.parent.mkdir(parents=True, exist_ok=True)
            existing_backup = None
            if destination.exists():
                shutil.copy2(destination, backup)
                existing_backup = backup
            temporary = destination.with_name(f".{destination.name}.codegen-new-{os.getpid()}")
            temporary_files.append(temporary)
            shutil.copy2(source, temporary)
            temporary.replace(destination)
            temporary_files.remove(temporary)
            replaced_files.append((destination, existing_backup))
    except Exception:
        for temporary in temporary_files:
            temporary.unlink(missing_ok=True)
        for destination, backup in reversed(replaced_files):
            if backup is None:
                destination.unlink(missing_ok=True)
            else:
                shutil.copy2(backup, destination)
        for destination, backup in reversed(replaced_directories):
            if destination.exists():
                shutil.rmtree(destination)
            if backup is not None:
                backup.replace(destination)
        raise


def generate_project(root: Path, check: bool = False, verbose: bool = True) -> bool:
    """Generate the project. Return True when checked/applied output is clean."""
    root = root.resolve()
    required = (root / "codegen/config", root / "codegen/templates", root / "src/app")
    missing = [path for path in required if not path.exists()]
    if missing:
        raise FileNotFoundError("missing project path(s): " + ", ".join(map(str, missing)))

    with tempfile.TemporaryDirectory(prefix=".codegen-stage-", dir=root) as temporary:
        staging_root = Path(temporary)
        prepare_staging_tree(root, staging_root)
        if verbose:
            render_to_staging(root, staging_root)
        else:
            with contextlib.redirect_stdout(io.StringIO()):
                render_to_staging(root, staging_root)
        changes = collect_changes(root, staging_root)

        if check:
            if changes:
                print_section("generated output is stale")
                print_changes(changes, root, staging_root)
                return False
            print_section("generated output is up to date")
            return True

        if changes:
            print_section("applying validated generated output")
            print_changes(changes, root, staging_root)
        apply_staged_output(root, staging_root, changes)
        print_section("generation completed successfully")
        return True


def parse_args(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check",
        action="store_true",
        help="fail without modifying files when generated output is stale",
    )
    parser.add_argument(
        "--quiet",
        action="store_true",
        help="suppress per-file generation output",
    )
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[2],
        help=argparse.SUPPRESS,
    )
    return parser.parse_args(argv)


def main(argv=None) -> int:
    args = parse_args(argv)
    try:
        return 0 if generate_project(args.root, check=args.check, verbose=not args.quiet) else 1
    except Exception as error:
        print(f"code generation failed: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
