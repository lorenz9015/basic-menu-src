"""Build the local HTML with an embedded, freestanding WebAssembly reader."""
import argparse
import base64
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def build(out):
    out.mkdir(parents=True, exist_ok=True)
    wasm = out / "log.wasm"
    compiler = os.environ.get("WASM_CXX", "clang++-19")
    flags = [
        "--target=wasm32", "-std=c++17", "-Oz", "-Wall", "-Wextra", "-Wpedantic", "-Werror",
        "-ffreestanding", "-fno-exceptions", "-fno-rtti", "-fno-ident", "-nostdlib",
        "-Wl,--no-entry", "-Wl,--strip-all", "-Wl,--import-undefined",
        "-Wl,--export=log_input", "-Wl,--export=log_capacity", "-Wl,--export=inspect_log",
        "-Wl,--export-memory",
    ]
    subprocess.run([compiler, *flags, str(ROOT / "diagnostic/log.cpp"), "-o", str(wasm)], check=True)
    replacements = {
        "/*__READER__*/": (ROOT / "diagnostic/report.js").read_text(encoding="utf-8"),
        "/*__RELEASES__*/": json.dumps(json.loads((ROOT / "data/releases.json").read_text()), separators=(",", ":")),
        "/*__WASM__*/": base64.b64encode(wasm.read_bytes()).decode("ascii"),
    }
    html = (ROOT / "diagnostic/document.html").read_text(encoding="utf-8")
    for key, value in replacements.items():
        if html.count(key) != 1:
            raise ValueError(f"Template marker missing or ambiguous: {key}")
        html = html.replace(key, value)
    (out / "Diagnose.html").write_bytes(html.encode("utf-8"))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, default=ROOT / "build")
    build(parser.parse_args().out)
