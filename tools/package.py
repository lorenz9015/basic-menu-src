"""Create the deterministic host diagnostics archive and its external checksum."""
import argparse
from hashlib import sha256
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]
SOURCES = (
    ".github/workflows/diagnose.yml", ".gitignore", "README.md", "BEFUNDE.md", "STATUS.md",
    "CONTINUE.md", "LICENSE", "data/releases.json", "diagnostic/log.cpp", "diagnostic/report.js",
    "diagnostic/document.html", "tests/native.cpp", "tests/report.test.cjs", "tests/browser.py",
    "tests/browser-requirements.txt", "tools/build.py", "tools/check.sh", "tools/package.py",
    "tools/verify_releases.py",
)
NAME = "basic-menu-diagnose-13.60.zip"


def package(build_dir):
    files = {name: (ROOT / name).read_bytes() for name in SOURCES}
    files["Diagnose.html"] = (build_dir / "Diagnose.html").read_bytes()
    lines = [f"{sha256(data).hexdigest()}  {name}\n" for name, data in sorted(files.items())]
    files["SHA256SUMS"] = "".join(lines).encode()
    out = build_dir / NAME
    with zipfile.ZipFile(out, "w", compression=zipfile.ZIP_STORED) as archive:
        for name, data in sorted(files.items()):
            entry = zipfile.ZipInfo(name, date_time=(2026, 11, 27, 0, 0, 0))
            entry.create_system = 3
            entry.external_attr = (0o100755 if name == "tools/check.sh" else 0o100644) << 16
            archive.writestr(entry, data)
    digest = sha256(out.read_bytes()).hexdigest()
    (build_dir / (NAME + ".sha256")).write_bytes(f"{digest}  {NAME}\n".encode())
    print(digest, out)


if __name__ == "__main__":
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("--build-dir", type=Path, default=ROOT / "build")
    package(cli.parse_args().build_dir)
