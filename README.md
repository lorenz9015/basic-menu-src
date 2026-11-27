# Basic Menu · Prüfstand für PS5-Firmware 13.60

**Der einzige Hardwaretest ist zurückgestellt. Für die PS5 gibt es aus diesem Prüfstand kein installierbares Mod-Menü.** Dieser Zweig enthält die lokale Auswertung für die Originalveröffentlichungen und das Menüprotokoll. Das ursprüngliche Story-Modus-Menü für GTA VI stammt von XeMo. [Originalprojekt](https://github.com/OnlyMoisties/basic-menu-src)

## Lesen oder selbst auswerten

1. Das Diagnose-ZIP auf dem **Rechner** entpacken. Der Befund für das Zielsystem steht in `STATUS.md`.
2. Optional `Diagnose.html` per Doppelklick in einem aktuellen Browser öffnen. Internetzugang und Installation entfallen. Für den bereits erstellten Befund brauchst du keine eigenen Dateien.
3. Falls du bereits eine `menu.log` oder eine der beiden Original-PRX-Dateien besitzt, kannst du sie auswählen. Das Protokoll muss als Text mit den ursprünglichen Zeilenumbrüchen vorliegen. Die Grenze beträgt 4 MiB pro Datei. Die Ergebnisse erscheinen direkt unter dem jeweiligen Auswahlfeld. Zum Entfernen der Diagnose das Browserfenster schließen und den entpackten Ordner sowie das ZIP löschen.

Die HTML-Datei liest nur die von dir gewählten lokalen Dateien. Sie verbindet sich weder mit der PS5 noch mit GitHub. Es gibt keine Installation, Startsequenz oder Deinstallation auf der Konsole. Die PS5 kann während der Auswertung ausgeschaltet bleiben.

## Inhalt

| Datei | Zweck |
|---|---|
| `STATUS.md` | Abnahmestand, Fehler und Entscheidung zum einmaligen Hardwaretest |
| `BEFUNDE.md` | Quellbelege, Firmware- und Buildanalyse, durchgeführte Prüfungen |
| `CONTINUE.md` | Übergabe, Prüfbefehle und Voraussetzungen für die Weiterarbeit |
| `Diagnose.html` | Fertige, lokal ausführbare Diagnose im ZIP |
| `diagnostic/`, `tests/`, `tools/`, `data/` | Quellcode, Tests, Build sowie Referenzwerte |
| `SHA256SUMS` | SHA-256 aller übrigen Dateien im ZIP |

Die MIT-Lizenz in diesem Zweig gilt für die eigenständig erstellte Diagnose. Das Werk von XeMo, Spielinhalte, Sony-SDKs, etaHEN, kstuff und PRX-Dateien sind nicht Bestandteil des Diagnose-ZIP. Die vom Originalautor genannten Nutzungsbedingungen stehen im Originalprojekt. [Upstream-README](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/README.md)
