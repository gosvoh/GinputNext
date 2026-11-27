"""Verify the exact Windows SDL profile shipped with the SA package."""
from pathlib import Path
import sys

guid = "030000006d04000016c2000000000000"
rows = [line.split(",") for line in Path(sys.argv[1]).read_text(encoding="utf-8").splitlines()
        if line.startswith(guid + ",") and ",platform:Windows," in line]
assert len(rows) == 1, rows
bindings = dict(field.split(":", 1) for field in rows[0][2:] if field)
for index, logical in enumerate("x a b y leftshoulder rightshoulder lefttrigger righttrigger back start leftstick rightstick".split()):
    assert bindings[logical] == f"b{index}", logical
for index, logical in enumerate(("leftx", "lefty", "rightx", "righty")):
    assert bindings[logical] == f"a{index}", logical
for mask, logical in ((1, "dpup"), (2, "dpright"), (4, "dpdown"), (8, "dpleft")):
    assert bindings[logical] == f"h0.{mask}", logical
print("Windows Logitech Dual Action profile verified.")
