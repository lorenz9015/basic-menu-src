"""Exercise the actual file chooser on the local, packaged page with the network off."""
import argparse
from hashlib import sha256
from pathlib import Path
import shutil
from urllib.parse import urlsplit

from playwright.sync_api import expect, sync_playwright

ROOT = Path(__file__).resolve().parents[1]


def run(html, assets, memory_document):
    binary = shutil.which("google-chrome") or shutil.which("chromium")
    if not binary:
        raise SystemExit("Chromium or Google Chrome is required for this browser test.")
    external = []
    errors = []
    test_url = "http://127.0.0.1/Diagnose.html"
    with sync_playwright() as playwright:
        browser = playwright.chromium.launch(executable_path=binary)
        context = browser.new_context(offline=True)
        page = context.new_page()
        page.on("request", lambda request: external.append(request.url)
                if urlsplit(request.url).scheme in ("http", "https", "ws", "wss")
                and not (memory_document and request.url == test_url) else None)
        page.on("pageerror", lambda error: errors.append(str(error)))
        if memory_document:
            # Used only where managed Chromium blocks file URLs. The response
            # is fulfilled from bytes in the test harness, without a server.
            context.route(test_url, lambda route: route.fulfill(
                body=html.read_bytes(), content_type="text/html; charset=utf-8"))
            page.goto(test_url)
        else:
            page.goto(html.resolve().as_uri())
        log = page.locator("#log")
        result = page.locator("#log-result")

        def set_log(content):
            log.set_input_files({"name": "menu.log", "mimeType": "text/plain", "buffer": content})

        expect(page.get_by_text("Hardwaretest zurückgestellt.", exact=True)).to_be_visible()
        sample = (b"[     2.111] Basic Menu, built Nov 26 2026 14:09:05\r\n"
                  b"[     3.172] NP state 0 (ret 0x80123456), reachability -1 (ret 0x0) -> offline, features allowed\r\n"
                  b"[     3.180] overlay: no writable display buffer\r\n")
        set_log(sample)
        expect(result).to_contain_text("0x80123456")
        expect(result).to_contain_text("unbestimmt")
        expect(result).to_contain_text("kein beschreibbarer Bildpuffer")
        set_log(b"\0")
        expect(result).to_contain_text("Nullbytes")
        expect(result).not_to_contain_text("0x80123456")
        set_log(b"x" * (4194304 + 1))
        expect(result).to_contain_text("zu gro")
        set_log(b"")
        expect(result).to_contain_text("Keine vollständige")
        log.set_input_files([])
        expect(result).to_contain_text("Kein Protokoll ausgewählt")
        pick = page.locator("#asset")
        output = page.locator("#asset-result")
        pick.set_input_files({"name": "BasicMenu.prx", "mimeType": "application/octet-stream", "buffer": b"abc"})
        expect(output).to_contain_text(sha256(b"abc").hexdigest())
        expect(output).to_contain_text("Kein Treffer")
        for path, tag in assets:
            pick.set_input_files(str(path))
            expect(output).to_contain_text("Originaldatei aus " + tag)
            expect(output).to_contain_text(sha256(path.read_bytes()).hexdigest())
        browser.close()
    if errors or external:
        raise AssertionError((errors, external))
    mode = "in-memory document" if memory_document else "local file URL"
    print(f"Browser flow passed ({mode}); {len(assets)} original release assets checked; 0 external requests.")


if __name__ == "__main__":
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("--html", type=Path, default=ROOT / "build/Diagnose.html")
    cli.add_argument("--memory-document", action="store_true")
    cli.add_argument("--asset", nargs=2, metavar=("PATH", "TAG"), action="append", default=[])
    args = cli.parse_args()
    run(args.html, [(Path(p).resolve(), tag) for p, tag in args.asset], args.memory_document)
