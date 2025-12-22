from pathlib import Path
import re

_BEGIN_RE = re.compile(
    r"//\s*=+\s*GENERATED\s+BEGIN:\s*([A-Za-z0-9_]+)\s*=+"
)

_END_RE = re.compile(
    r"//\s*=+\s*GENERATED\s+END:\s*([A-Za-z0-9_]+)\s*=+"
)

class GenSectionError(RuntimeError):
    pass

class GenSection:
    def __init__(self, filename: str, section_name: str, config: dict, template_path: Path):
        if not section_name:
            raise ValueError("section_name must not be empty")

        self.filename = filename
        self.section_name = section_name
        self.config = config
        self.template_path = template_path


    def apply(self, result_text: str, section_text: str) -> str:
        section_name = self.section_name
        lines = result_text.splitlines(keepends=True)
        out = []

        in_target = False
        begin_found = False
        end_found = False

        for line in lines:
            m_begin = _BEGIN_RE.search(line)
            m_end = _END_RE.search(line)

            if m_begin:
                name = m_begin.group(1)
                if name == section_name:
                    if begin_found:
                        raise GenSectionError(
                            f"Duplicate GENERATED BEGIN for section '{section_name}' in file '{self.filename}'"
                        )
                    begin_found = True
                    in_target = True
                    out.append(line)
                    out.append(section_text)
                    if not section_text.endswith("\n"):
                        out.append("\n")
                    continue

            if m_end:
                name = m_end.group(1)
                if name == section_name:
                    if not begin_found:
                        raise GenSectionError(
                            f"gen:end without GENERATED BEGIN for section '{section_name}' in file '{self.filename}'"
                        )
                    end_found = True
                    in_target = False
                    out.append(line)
                    continue

            if not in_target:
                out.append(line)

        if not begin_found:
            raise GenSectionError(
                f"Missing GENERATED BEGIN for section '{section_name}' in file '{self.filename}'"
            )

        if not end_found:
            raise GenSectionError(
                f"Missing GENERATED END for section '{section_name}' in file '{self.filename}'"
            )

        return "".join(out)
