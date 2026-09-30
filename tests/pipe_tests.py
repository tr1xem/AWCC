#!/usr/bin/env python3
"""Pipe tests for awcc.

Each case starts the binary, writes bytes to its stdin, and checks the exit
code plus stdout and stderr. Nothing here names a laptop, a USB id, or a
zone map. --test-mode keeps device lookup from exiting on other machines.
"""

import subprocess
import sys


def run(binary, args, stdin):
    completed = subprocess.run(
        [binary, *args],
        input=stdin,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=20,
        check=False,
    )
    out = completed.stdout.decode("utf-8", "replace")
    err = completed.stderr.decode("utf-8", "replace")
    return completed.returncode, out, err


def expect(binary, args, stdin, code, out_has, err_has):
    status, out, err = run(binary, args, stdin)
    problems = []
    if status != code:
        problems.append(f"exit {status}, expected {code}")
    for needle in out_has:
        if needle not in out:
            problems.append(f"stdout missing {needle!r}")
    for needle in err_has:
        if needle not in err:
            problems.append(f"stderr missing {needle!r}")
    if problems:
        print(f"FAIL {' '.join(args) or '(no args)'}")
        for problem in problems:
            print(f"  {problem}")
        print("--- stdout ---")
        print(out)
        print("--- stderr ---")
        print(err)
        return 1
    print(f"ok {' '.join(args) or '(no args)'}")
    return 0


def main():
    if len(sys.argv) != 2:
        print("usage: pipe_tests.py <awcc>", file=sys.stderr)
        return 2
    binary = sys.argv[1]
    help_bits = (
        "Alienware Command Center",
        "App Commands",
        "Keyboard Lighting Controls",
        "Fan Controls",
    )
    cases = [
        (["--test-mode", "-h"], b"", 0, help_bits, ()),
        (["--test-mode", "--help"], b"\n", 0, help_bits, ()),
        (["--test-mode"], b"", 0, help_bits, ()),
        (
            ["--test-mode", "not-a-real-command"],
            b"",
            1,
            (),
            ("Unknown command",),
        ),
    ]
    failed = 0
    for args, stdin, code, out_has, err_has in cases:
        failed += expect(binary, args, stdin, code, out_has, err_has)
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
