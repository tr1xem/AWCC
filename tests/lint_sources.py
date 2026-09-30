#!/usr/bin/env python3
"""Source lint that does not need a compiler or a particular machine.

Checks C and C++ sources for trailing whitespace, CR bytes, a missing final
newline, and git conflict markers. A banner of '=' characters is not a
conflict marker.
"""

import os
import sys

SKIP_DIRS = {"build", ".git", "_deps", "FetchContent", ".cache"}
SUFFIXES = {".c", ".cc", ".cpp", ".h", ".hpp"}


def problems_in(path):
    data = open(path, "rb").read()
    found = []
    if b"\r" in data:
        found.append("contains CR")
    if data and not data.endswith(b"\n"):
        found.append("does not end with a newline")
    for number, line in enumerate(data.splitlines(), 1):
        if line.rstrip(b" \t") != line:
            found.append(f"trailing whitespace on line {number}")
            break
    for number, line in enumerate(data.splitlines(), 1):
        if (
            line.startswith(b"<<<<<<<")
            or line.startswith(b">>>>>>>")
            or line == b"======="
        ):
            found.append(f"conflict marker on line {number}")
            break
    return found


def main():
    if len(sys.argv) != 2:
        print("usage: lint_sources.py <source-root>", file=sys.stderr)
        return 2
    root = sys.argv[1]
    failed = 0
    checked = 0
    for dirpath, dirnames, filenames in os.walk(root):
        dirnames[:] = [
            name
            for name in dirnames
            if name not in SKIP_DIRS and not name.startswith(".")
        ]
        for name in filenames:
            if os.path.splitext(name)[1] not in SUFFIXES:
                continue
            path = os.path.join(dirpath, name)
            checked += 1
            found = problems_in(path)
            if found:
                failed += 1
                print(path)
                for item in found:
                    print(f"  {item}")
    if failed:
        print(f"{failed} file(s) failed lint ({checked} checked)")
        return 1
    print(f"ok {checked} files")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
