import re

def preprocess_xml_text(xml_text):
    xml_text = preprocess_components(xml_text)
    xml_text = preprocess_root(xml_text)
    return xml_text


def preprocess_components(xml_text):
    pat = re.compile(r"(<components\b[^>]*>)(.*?)(</components>)", re.DOTALL)
    m = pat.search(xml_text)
    if not m:
        return xml_text

    start, inner, end = m.groups()
    
    # Convert Optional Params
    # Must trigger only in attribute names of tags
    # Example: $foo="value" => o-foo="value"
    inner = re.sub(
        r'(?<=\s)\$(\w+)\s*=\s*"([^"]*)"',   # $foo="bar"
        lambda m: f'o-{m.group(1)}="{m.group(2)}"',
        inner
    )

    # Convert Required Params
    # Must trigger only in attribute names of tags
    # Example: $foo => r-foo=""
    inner = re.sub(
        r'(?<=\s)\$(\w+)(?=\s|>|\/>)',       # bare $foo
        lambda m: f'r-{m.group(1)}=""',
        inner
    )

    # Convert Attribute Bindings
    # Must trigger only in attribute values, not strings
    # Example: foo=$bar => v-foo="bar"
    inner = re.sub(
        r'(\s)([\w-]+)\s*=\s*\$(\w+)',
        lambda m: f'{m.group(1)}v-{m.group(2)}="{m.group(3)}"',
        inner
    )

    return xml_text[:m.start()] + start + inner + end + xml_text[m.end():]


def preprocess_root(xml_text):
    pat = re.compile(r"(<root\b[^>]*>)(.*?)(</root>)", re.DOTALL)
    m = pat.search(xml_text)
    if not m:
        return xml_text

    start, inner, end = m.groups()
    
    # Outside-components rule: $foo="bar" → p-foo="bar"
    inner = re.sub(
        r'\s\$(\w+)\s*=\s*"([^"]*)"',
        lambda m: f' p-{m.group(1)}="{m.group(2)}"',
        inner
    )

    return xml_text[:m.start()] + start + inner + end + xml_text[m.end():]
