# Übergabe für die nächste Sitzung

Der Produktauftrag bleibt offen. Der Zweig `qa/firmware-13.60` im öffentlichen Fork enthält die eigenständig entwickelte Hostdiagnose, ihre Tests und die Befunde. Er hat eine eigene Historie. Das Original und der Hauptzweig des Forks wurden nicht verändert. Als Referenz dienen Upstream-Commit `9bce467bd39eb5d5d8facfec6eaae1e41fc44521` und die zwei in `data/releases.json` festgehaltenen Assets. [Diagnosezweig](https://github.com/lorenz9015/basic-menu-src/tree/qa/firmware-13.60) · [Originalstand](https://github.com/OnlyMoisties/basic-menu-src/tree/9bce467bd39eb5d5d8facfec6eaae1e41fc44521)

## Nächste fachliche Entscheidung

1. Neue Erkenntnisse zuerst an B1–B6 in `STATUS.md` messen. Issue 1, neue Releases und Veränderungen der untersuchten Forks seit dem 27. November 2026 auf einen **diesem Modul zuordenbaren** Befund prüfen. Den bisherigen Stand und die Quellen nennt `BEFUNDE.md`.
2. Für den PS5-Teil fehlen überprüfbare Build-Eingaben, klare Bearbeitungs- und Verteilungsrechte sowie eine dokumentierte, im Zielkontext zulässige Modulschnittstelle. Ein vorhandener FTP-Zugang und das allgemeine Firmwareprofil eines Homebrew-Projekts belegen diese Voraussetzungen nicht. Nützliche externe Eingaben wären Unterlagen des Maintainers zur Toolchain und ein bereits existierender, versionsgebundener Startbericht. Zwischenversuche auf der Konsole des Auftraggebers sind hierfür nicht vorgesehen.
3. Bei vorliegenden Voraussetzungen zuerst den sicheren Abbruch und das Beenden nach Teilfehlern testen. Dazu gehören fehlende Versionsdaten, unbekannter Spielbuild, Fehler bei der Offline-Prüfung, fehlende Anzeigenachweise und das Ausbleiben der ersten Modulmeldung. Ein Diagnoseexport muss explizit zwischen „nicht beobachtet“, „fehlgeschlagen“ und „erfolgreich gemeldet“ unterscheiden.
4. Erst mit geeignetem Installationspaket eine Anleitung für den einmaligen Hardwaretest ausgeben. Sie muss die tatsächlich gebauten Dateinamen und Installationsorte, die automatische Vorprüfung, wenige sichtbare reversible Menüoperationen, den Diagnoseexport und die Entfernung der installierten Dateien angeben. Für die aktuelle Version ist nur die Rechnerdiagnose verfügbar. Jede nach einem Hardwaretest geänderte Binärdatei hat einen neuen Prüfstatus.

## Diagnose reproduzieren

Für Anwender genügt die fertige `Diagnose.html` im ZIP. Die folgenden Befehle sind für Entwicklungs- und CI-Umgebungen bestimmt.

Referenzumgebung war Debian 13 x86-64 mit GCC 14.2.0, Clang/LLD 19.1.7, Python 3.12, Node 24 und Chromium 156. Die LLVM-Pakete kamen aus dem signierten Debian-Archiv. Für die Browsersteuerung verwendet die CI ein Python-3.12-Environment und die in `tests/browser-requirements.txt` festgelegten Linux-Wheels mit SHA-256. Für die ausgelieferte HTML-Datei werden diese Werkzeuge nicht benötigt.

```sh
python3.12 -m venv /tmp/basic-menu-test-venv
. /tmp/basic-menu-test-venv/bin/activate
python -m pip install --only-binary=:all: --require-hashes -r tests/browser-requirements.txt
tools/check.sh
python3 tools/package.py
(cd build && sha256sum -c basic-menu-diagnose-13.60.zip.sha256)
```

`tools/check.sh` benötigt `g++`, `clang++-19`, `wasm-ld-19` und Chromium oder Google Chrome im Suchpfad. Bei abweichender Installation bezeichnet `WASM_CXX` den Clang-Treiber. Für Browserumgebungen mit gesperrten Datei-URLs prüft `tools/check.sh --memory-document` dieselben HTML-Bytes und die Dateiauswahl mit einer im Testprogramm erzeugten Antwort. Die Ausführung über eine Datei-URL bleibt ein eigener Testfall.

Für einen unabhängigen zweiten Paketbau genügen die folgenden Befehle nach dem ersten Build. Feste Eintragsreihenfolge, Zeitstempel und Dateimodi sowie unkomprimierte ZIP-Einträge machen den Vergleich unabhängig von der Kompressionsbibliothek. Bytegleichheit wurde innerhalb derselben Toolchain geprüft.

```sh
second_build=$(mktemp -d)
python3 tools/build.py --out "$second_build"
python3 tools/package.py --build-dir "$second_build"
cmp build/basic-menu-diagnose-13.60.zip "$second_build/basic-menu-diagnose-13.60.zip"
```

`python3 tools/verify_releases.py --github` prüft nur öffentliche Metadaten der beiden eingefrorenen Upstream-Releases. Zusätzliche lokale Dateipfade werden im Lesemodus geprüft. Die Werkzeuge laden kein PRX-Modul. Mit `python3 tests/browser.py --asset DATEIPFAD TAG` lässt sich jeweils auch der Browservergleich wiederholen. `--asset` ist wiederholbar. Bei einem künftigen Upstream-Release Referenzwerte und Nachweise separat verifizieren.

## Nachweise auf GitHub

Die Commits im Diagnosezweig und die Prüfmatrix in `STATUS.md` bilden den Arbeitsstand ab. Der Workflow baut ausschließlich die Diagnose, führt die Tests aus und vergleicht zwei Paketbauten. Das lokale Build-Ergebnis, der Status eines gehosteten Workflow-Laufs und die Abnahme des Konsolenmenüs sind getrennte Nachweise. [Workflow](https://github.com/lorenz9015/basic-menu-src/blob/qa/firmware-13.60/.github/workflows/diagnose.yml)
