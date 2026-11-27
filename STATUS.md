# Entwicklungsstand am 27. November 2026

**PS5-Releasekandidat gesperrt.** Das bereitgestellte Original bezieht sich auf GTA VI für PS5. XeMo dokumentiert Tests ausschließlich für Firmware 7.40 und die Kombination `PPSA38500 / 01.002.001`. Für das konkrete Menü auf Firmware 13.60 liegen im geprüften Upstream zwei Fehlermeldungen und kein erfolgreicher, dieser Binärdatei zuordenbarer Durchlauf vor. Deine Meldung über etaHEN und kstuff beschreibt die Ausgangslage. Versionen der installierten Komponenten und Spielbuild auf deiner Konsole wurden nicht gemessen. [Upstream-Release](https://github.com/OnlyMoisties/basic-menu-src/releases/tag/v1.1-2026-11-26) · [Issue 1 und Kommentare](https://github.com/OnlyMoisties/basic-menu-src/issues/1)

## Was vorliegt

| Abnahmekriterium | Stand |
|---|---|
| Upstream-Quellen und Historie | Bestand und erreichbare Historie eingefroren. Build-Eingaben, Startpfad und kritische Schutzprüfungen untersucht. Funktionsweise sämtlicher Einzeloptionen auf 13.60 offen. |
| Vollständiger PRX-Build | Offen. Der öffentliche Stand enthält kein Buildrezept. Verwendete SDK-Header, passende Bibliotheken, Link- und Paketierungsparameter sind nicht vollständig ausgewiesen. |
| Stabilität und reine Offline-Nutzung | Gesperrt. Die Spielprüfung schützt Teile der Engine. Der Modulstart kann trotzdem Schnittstellen verändern. Fehler bei der PSN-Abfrage können als offline gelten. |
| Erforderliche Komponenten | Der Autor veröffentlicht eine PRX. Ein für diese genaue Kombination belegter Ladeweg und reproduzierbare Build-Eingaben fehlen. |
| Binärintegrität | Beide Upstream-Assets lokal gehasht, Länge und GitHub-Metadaten abgeglichen. |
| Diagnose und Host-QA | Eigenständiger C++-Leser nativ und als WebAssembly gebaut, statisch geprüft und mit Sanitizern, Integrationstests und Browser-Dateiauswahl getestet. |
| Paket | Fertige lokale Rechnerdiagnose mit Quellcode, Tests, deutschen Anleitungen und Prüfsummen. Eine PS5-Installation lässt sich damit nicht durchführen. |
| Funktions-, Abbruch- und Deinstallationstest auf der PS5 | Ausstehend. Keine Änderungen an deiner Konsole. |
| Abschließender Hardwaretest | Unverbraucht. Die Bedingungen für einen sinnvollen Einzeldurchlauf sind derzeit nicht erfüllt. |

## Offene Fehler und Nachweise

| Punkt | Befund | Was für eine Freigabe fehlt |
|---|---|---|
| B1 · Spiel- und Firmwareidentität | Bildgröße und Tabellenvergleich identifizieren die erwartete Speicherstruktur nur indirekt. | Exakte Versionsdaten und sichere Ablehnung einer unbekannten Kombination vor jedem Eingriff. |
| B2 · Start und Ende | Ergebnisse einzelner Startschritte bleiben unbeachtet. Der Arbeits-Thread läuft losgelöst weiter. Die Stopproutine meldet Erfolg ohne Aufräumen. | Reproduzierbarer Abbruch, Rücknahme bereits erfolgreicher Schritte und nachprüfbarer Abschluss. |
| B3 · Anzeige | Bei fehlender Registrierung verwendet das Original feste Pufferannahmen. Beschreibbarkeit belegt weder Herkunft noch aktives Bildformat. | Verlässliche Herkunft, Laufzeitformat und Abmessungen sowie sichere Ablehnung unpassender Puffer. |
| B4 · Offline-Zustand | Ein Fehler in der Netzabfrage kann Funktionen freigeben. Eine positive Story-Modus-Erkennung ist in der geprüften Freigabeentscheidung nicht erkennbar. | Verhalten bei unbekanntem Zustand und Verbindungswechseln, einschließlich Tests gegen versehentliche Freigabe. |
| B5 · Build und Ladeweg | Upstream-Rezept fehlt. Beide Fehlerberichte lassen die genaue Kombination von Moduldatei, Loader und Spielbuild offen. | Nachvollziehbare, zulässige Build-Eingaben und dokumentierte Modulschnittstelle für das Ziel. |
| B6 · Diagnose vor Modulstart | Ein Scheitern vor der Dateiprotokollierung kann ein leeres oder fehlendes Log hinterlassen. | Ein von der Modullaufzeit unabhängiger Startbeleg und eine gemeinsame Kennung für die Diagnosedaten eines Durchlaufs. |

Die Quellstellen, Paketkennungen und Messungen zu B1–B6 stehen im [Prüfbericht](BEFUNDE.md).

## Entscheidung für deinen einzigen Test

**Jetzt nichts auf die PS5 kopieren oder starten.** Ein Absturz des Spiels und ein nicht sauber beendetes Modul sind plausible Folgen. Ein persistenter Konsolenschaden ist durch diese Untersuchung nicht belegt. Erfolgswahrscheinlichkeit und Risiko eines Spielstandverlusts lassen sich aus den vorhandenen Daten nicht beziffern. Auch ein ausbleibendes Menü oder ein fehlendes Log würde die Ursache nicht eindeutig eingrenzen.

Für die gelieferte Diagnose reicht das Entpacken auf dem Rechner. Die Auswertung selbst berührt Konsole und Spielstände nicht. Für einen späteren qualifizierten Kandidaten soll dieselbe Sitzung Installation, automatische Vorprüfung, sichtbare Funktionsprobe, Diagnoseexport und kontrolliertes Beenden abdecken. Konkrete Installations- und Entfernungsschritte müssen zum dann geprüften Paket passen.
