"""Verify local reference assets and optionally check GitHub metadata without loading any module."""
import argparse
import hashlib
import json
from pathlib import Path
from urllib.request import urlopen

ROOT = Path(__file__).resolve().parents[1]
REPO = "https://api.github.com/repos/OnlyMoisties/basic-menu-src"


def check_metadata(entry):
    with urlopen(REPO + "/releases/tags/" + entry["tag"], timeout=20) as response:
        release = json.load(response)
    candidates = [a for a in release["assets"] if a["name"] == "BasicMenu.prx"]
    expected = (entry["assetId"], entry["bytes"], "sha256:" + entry["sha256"])
    if len(candidates) != 1:
        raise ValueError("Unexpected number of BasicMenu.prx assets")
    actual = candidates[0]
    if (actual["id"], actual["size"], actual["digest"]) != expected:
        raise ValueError("Release metadata changed for " + entry["tag"])
    with urlopen(REPO + "/git/ref/tags/" + entry["tag"], timeout=20) as response:
        target = json.load(response)["object"]
    if target["type"] != "commit" or target["sha"] != entry["commit"]:
        raise ValueError("Tag target changed for " + entry["tag"])
    print(entry["tag"], "asset ID, size, digest and tag target match GitHub")


def check_asset(path, catalog):
    with path.open("rb") as stream:
        prefix = stream.read(4).hex()
        stream.seek(0)
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
        size = stream.seek(0, 2)
    matches = [e for e in catalog if e["sha256"] == digest and e["bytes"] == size]
    if len(matches) != 1:
        raise ValueError("Unknown file contents: " + path.name)
    if prefix != "4f153d1d":
        raise ValueError("Unexpected container prefix")
    print(matches[0]["tag"], size, digest, "prefix", prefix)
    return matches[0]["tag"]


if __name__ == "__main__":
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("--github", action="store_true", help="compare the pinned release metadata with GitHub")
    cli.add_argument("files", nargs="*", type=Path)
    args = cli.parse_args()
    if not args.github and not args.files:
        cli.error("give local files or --github")
    catalog = json.loads((ROOT / "data/releases.json").read_text())
    if args.github:
        for record in catalog:
            check_metadata(record)
    for path in args.files:
        check_asset(path, catalog)
