# Prüfbericht für GTA VI, PS5-Firmware 13.60

Stand der Upstream-Erhebung am 27. November 2026. Der Befund unterscheidet Angaben der Projektbetreiber, am Rechner reproduzierte Messungen und offene Fragen.

## Original und direkte Forks

Vollständiger Git-Klon des öffentlich erreichbaren Originalbestands. `git fsck --full` lief erfolgreich. Der Standardzweig `main` stand bei `9bce467bd39eb5d5d8facfec6eaae1e41fc44521`. Er enthielt 29 getrackte Dateien, darunter 11 C++-Übersetzungseinheiten und 15 Header. GitHub meldete zwei Tags, zwei Releases, zwei Issues, keine Pull Requests sowie fünf direkte Forks. Die Listen wurden einschließlich geschlossener Issues und Pull Requests abgefragt. [Quellstand](https://github.com/OnlyMoisties/basic-menu-src/tree/9bce467bd39eb5d5d8facfec6eaae1e41fc44521) · [GitHub-Forks](https://github.com/OnlyMoisties/basic-menu-src/forks)

| Commit, ältester zuerst | Zweck laut Historie |
|---|---|
| `e58839d` | Erstveröffentlichung, Tag v1.0 |
| `d019118` | Nutzungsbedingungen im README |
| `8419ea7` | Quellhinweise hinzugefügt |
| `caea42d` | Quellhinweise entfernt |
| `367aee8` | Erweiterte Funktionen, Tag v1.1 |
| `9bce467` | Quellhinweise erneut hinzugefügt, zusätzlicher statischer Datenblock im Moduleinstieg |

Issue 1 enthält zwei Nutzerberichte. Einer nennt etaHEN. Der zweite beschreibt das Zurückspringen eines Schalters für eine als ELF bezeichnete Datei bei etaHEN und OnionHEN. Beide Berichte enthalten zu wenig Versions- und Ablaufdaten, um Loader, Container, Initialisierung oder Anzeige als Ursache festzulegen. Issue 2 fragt nach Anleitung und Sprachunterstützung. [Issue 1](https://github.com/OnlyMoisties/basic-menu-src/issues/1) · [Issue 2](https://github.com/OnlyMoisties/basic-menu-src/issues/2)

| Direkter Fork | Ergebnis vor Anlage des Diagnosezweigs |
|---|---|
| lorenz9015 | `main` identisch zum Original. Dieser öffentliche Fork gehört zum verbundenen Konto und ist beschreibbar. |
| dariwonka | `main` 28 Commits voraus. Änderungen unter anderem an Spielfunktionen, Fahrzeuganzeige und Quellhinweisen. Kein Release im Fork. [Letzter geprüfter Commit](https://github.com/dariwonka/basic-menu-src/commit/124f1031177ac0e2c9ec8b5c95b4e438f7d8b501) |
| Dr-Entropy | Ein zusätzlicher Commit mit leerer Workflowdatei. Der zugehörige Actions-Lauf schlug fehl. [Lauf](https://github.com/Dr-Entropy/basic-menu-src/actions/runs/45908533584) |
| xCENTx | `main` bei `8419ea7`, drei Upstream-Commits zurück. [Stand](https://github.com/xCENTx/basic-menu-src/tree/8419ea7c2c5a48177f9c63b0b72329c2e8dd6aa7) |
| insertwhatever | `main` vier Commits zurück. Zwei zusätzliche Zweige für Seitenwechsel sowie Fahrzeugfavoriten und Verlauf. [Zweige](https://github.com/insertwhatever/basic-menu-src/branches) |

Auf keinem dieser Forks fand sich zum Erhebungszeitpunkt ein Release mit einem belegten Durchlauf dieses Menüs auf 13.60. Darüber hinaus nennt ein **anderes Projekt** einen erfolgreichen Autorentest auf 13.60 für dieselbe Titel-/Buildkombination. Es handelt sich um einen extern bedienten Webtrainer mit einer anderen Laufzeitarchitektur. Damit gibt es einen Hinweis auf die Machbarkeit einzelner Spielaktionen, während Start, Eingabe und Anzeige der BasicMenu-PRX ungeklärt bleiben. Der angegebene Autorentest wurde hier nicht reproduziert. Das ebenfalls gefundene LeonidaPS5 beschreibt Tests seines Linux-Kommandozeilen-Emulators. [Projektangaben zum Webtrainer](https://github.com/deFIATer/GTA6-PS5-Web-Trainer/blob/32252e9b9396f47a8c998275074d7033df329bc9/README.md) · [Emulator-Projekt](https://github.com/crocodilocwb/LeonidaPS5/tree/87ee7c6605217bdba9cc54bfb161407064cda55c)

## Dateien, Herkunft und Rechte

Beide von GitHub veröffentlichten `BasicMenu.prx` wurden ohne Ausführung gelesen. Python/hashlib und die SHA-256-Implementierung des Browsers erkannten beide Fassungen. Tagziele, Asset-IDs, Dateilängen und Digests stimmten mit GitHubs API überein. [Release-Metadaten](https://api.github.com/repos/OnlyMoisties/basic-menu-src/releases)

| Tag | Byte | SHA-256 |
|---|---:|---|
| v1.1-2026-11-26 | 221265 | `700ea24e22e691fc046ef12bb56c13d935a28ae59722c0c3edaa20bab8c2b9ee` |
| v1.0-2026-11-26 | 47041 | `904afd8ce3886a7b736d89bc2e9580e9cee41859ee7fb03244502be4fa2d597c` |

Beide Dateien beginnen mit `4f 15 3d 1d`. Der externe Formatheader bezeichnet diese Kennung als SELF-Variante. GNU readelf erkennt am Dateianfang keinen normalen ELF-Header. Die vier Bytes erlauben keine Aussage über die Eignung für 13.60 oder die Ursache des beobachteten Ladefehlers. Die Hashwerte identifizieren die geprüften Bytes. Die Reproduzierbarkeit dieser PRX-Dateien aus einem bestimmten Commit bleibt offen. [Formatreferenz](https://github.com/ps5-payload-dev/ftpsrv/blob/92072f2694fdd804718cebfe0cc49fe7fe4aab82/self.h) · [Originalreleases](https://github.com/OnlyMoisties/basic-menu-src/releases)

Das Upstream-README verlangt Nennung von XeMo, einen öffentlichen Quellstand und unentgeltliche Weitergabe. Die aktuellen Quelldateien tragen zusätzlich einen Vorbehalt zur automatisierten Bearbeitung. Eine eigenständige, widerspruchsfrei anwendbare Lizenz für die angefragte Bearbeitung und Verteilung aller Bestandteile ist aus dem Bestand nicht ersichtlich. Das Diagnosepaket enthält nur die neu erstellten Werkzeuge und Dokumente. Fremde PRX-, Spiel-, SDK- und Loader-Dateien sind darin nicht enthalten. [README](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/README.md) · [Quellhinweis](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/main.cpp#L1)

## Build und Laufzeit

* **Build-Eingaben.** Weder Makefile, CMake-Projekt, Linkerskript, SDK-Lockdatei noch CI-Build liegen im Originalzweig. `make -n` endete mit Status 2 wegen des fehlenden Makefiles. Separate Compilerproben mit GCC 14 fanden auf dem Linux-Host `atomic`, `emmintrin.h` und `xmmintrin.h`. Die Proben für `kernel.h`, `pad.h`, `video_out.h`, `np/np_common.h` und `user_service/user_service_api.h` schlugen fehl. [Originalbaum](https://github.com/OnlyMoisties/basic-menu-src/tree/9bce467bd39eb5d5d8facfec6eaae1e41fc44521)
* **Öffentliche SDKs.** Das Payload-SDK v0.50 enthält unter anderem `include/ps5/kernel.h`. Die vier geprüften Pfade für Pad, Video, NP und UserService fehlen im rekursiven Git-Baum. Im geprüften etaHEN-Plugin-SDK fehlen ebenfalls diese Headerpfade. Das Plugin-SDK verweist seinerseits auf das Payload-SDK. Die Projekte dokumentieren Payloads bzw. Toolbox-Plugins. Daraus lässt sich kein ABI-identischer Ersatz für die ursprüngliche PRX-Toolchain ableiten. Beide Repositories weisen GPL-3.0 aus, mit zusätzlichen dateispezifischen Hinweisen. [Payload-SDK v0.50](https://github.com/ps5-payload-dev/sdk/tree/4122b714c7e7a9449d403812b9d640bf7a55af3f) · [Plugin-SDK](https://github.com/etaHEN/etaHEN-Plugins/tree/6339554e4e3c92c0a655a37194c1152d6b82ad88)
* **Firmwareberichte.** Im offiziellen etaHEN-Repository ist 2.5B als jüngstes reguläres Release geführt. Der eigenständige Port „2.5B Unified r1“ nennt 13.60 und Unterstützung für native Spielmodule. kstuff-lite v1.11 Beta nennt 1.00–13.60. Eine weitere inoffizielle Fassung dokumentiert einen Test auf einer Retail-PS5 für den Start von Homebrew im PS4-Format. Diese Ergebnisse decken unterschiedliche Vorgänge ab. [etaHEN 2.5B](https://github.com/etaHEN/etaHEN/releases/tag/2.5B) · [Port-Dokumentation](https://github.com/GronedWaffel/etahen-11.00-13.60/blob/89123ffbc9d14a4515c74800b00124595de60348/README.md) · [kstuff-lite v1.11](https://github.com/EchoStretch/kstuff-lite/releases/tag/v1.11) · [Testumfang der Community-Fassung](https://github.com/mushidani/etaHEN-13.60/blob/bced528978e45b5e141316256a0c268e7156317c/CREDITS.md)
* **Weitere Laufzeit.** OnionHEN v0.9.0 nennt Unterstützung der 13er-Reihe; zusätzlich existiert ein Nightly-Kanal. Der Fehlbericht im Menüprojekt benennt keine genaue OnionHEN-Version und keinen Modulhash. [OnionHEN v0.9.0](https://github.com/aydencharles/onionHEN/releases/tag/v0.9.0) · [Nutzerbericht](https://github.com/OnlyMoisties/basic-menu-src/issues/1#issuecomment-7213022415)

## Was der Startcode tatsächlich prüft

| Teil | Beobachtung im Quelltext | Technische Folge oder offene Frage |
|---|---|---|
| Modulstart | Der Moduleinstieg startet einen losgelösten Thread und gibt Erfolg zurück. Das Ergebnis der Thread-Erzeugung wird vor dem Ablösen nicht geprüft. Die Stopproutine gibt unmittelbar Erfolg zurück. | Ein früher Fehler kann vor dem ersten Log auftreten. Sicheres Entladen und Beenden des laufenden Threads sind offen. [Moduleinstieg](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/main.cpp#L94) |
| Reihenfolge | Die Videoanbindung läuft vor der Engine-Prüfung. Der Hauptthread ignoriert die Ergebnisse beider Aufrufe und versucht anschließend die Eingabeanbindung. | Ein gesperrter Funktionsbereich schließt frühere oder nachfolgende Änderungen beim Modulstart nicht aus. [Startpfad](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/main.cpp#L67) |
| Version und Aktionen | Die Engine prüft Abbildgröße, Lesbarkeit und ausgewählte Tabelleneinträge. Der Spieltick und die Auftragsannahme prüfen die daraus resultierende Freigabe. Viele Spielfunktionen und Datenstrukturen hängen am erwarteten Build. | Ein interner Treffer lässt nur auf die geprüften Merkmale schließen. Firmware, regionale Paket-ID und exakte Spielversion werden dadurch nicht automatisch erfasst. [Buildprüfung](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/Engine.cpp#L943) |
| Reine Offline-Nutzung | Die Netzentscheidung bildet die Verknüpfung aus gelungenen Abfragen, Anmeldung und Erreichbarkeit. Beim Fehler der Benutzerabfrage kehrt der Code vor einer Aktualisierung zurück. Fehler der zwei folgenden Abfragen können „offline“ ergeben. | Die Diagnose muss einen unbestimmten Zustand ausweisen. Der interne Zustand belegt auch bei erfolgreichen Abfragen keinen tatsächlichen Story-Modus. [Netzprüfung](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/Engine.cpp#L172) |
| Anzeige | Der Renderer erwartet ein bestimmtes Bildformat und verwendet bei fehlender Pufferregistrierung bekannte Adressen. Er prüft Schreibbarkeit und Bereichsgröße. | Diese Prüfung kann die Zugehörigkeit des Speichers zum aktuellen Bildpuffer nicht feststellen. Ein gemeldeter Handle belegt auch kein sichtbares Menü. [Pufferauswahl](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/Overlay.cpp#L124) |
| Diagnose und Dateien | Der Logger hängt Zeilen an die gleiche Datei an. Der Rückgabewert des Schreibens bleibt ungeprüft. Mehrere Startfehler werden über Systembenachrichtigungen gemeldet. Gespeicherte Orte haben eine separate Datei. | Eine fehlende Meldung lokalisiert keinen Absturz. Alte Starts dürfen nicht als Befund des letzten Laufs erscheinen. Das Löschen einer Datei würde Änderungen in einem Spielstand nicht rückgängig machen. [Logger](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/Log.cpp#L16) · [Startmeldungen](https://github.com/OnlyMoisties/basic-menu-src/blob/9bce467bd39eb5d5d8facfec6eaae1e41fc44521/src/main.cpp#L60) |

**Ursache des 13.60-Fehlers offen.** Das öffentliche Material enthält keinen zusammenhängenden Trace vom Ladeversuch bis zum sichtbaren Menü mit passender Firmware-, Spiel-, Loader- und Modulidentität. Die statischen Befunde sind konkrete Gründe, die veröffentlichte PRX für den gewünschten Einmaltest zurückzustellen. Ob einer von ihnen den von anderen Nutzern beobachteten Fehler ausgelöst hat, bleibt offen.

## Eigene Implementierung und Tests

Der neue C++17-Leser verarbeitet nur den übergebenen Dateiinhalt. Die WebAssembly-Version importiert genau eine Funktion für fest definierte Ereignisnamen und Zahlenwerte. Sie importiert keine Datei-, Prozess- oder Netzwerkschnittstelle. Die HTML-Datei enthält Leser und Referenzkatalog vollständig. Benutzertexte, Pfade und Speicheradressen werden nicht in den Bericht kopiert. Eine Menüoperation im Protokoll beweist ihren sichtbaren Effekt im Spiel nicht.

| Tatsächlich lokal geprüft | Ergebnis und Reichweite |
|---|---|
| C++17 nativ und WebAssembly | GCC 14.2.0 und Clang/LLD 19.1.7. Warnungen als Fehler. Beide Varianten gebaut. |
| Statische Analyse | GCC `-fanalyzer` und Clang Static Analyzer auf dem Diagnoseleser. Kein gemeldeter Befund. |
| Speicher- und Fehlerfälle | Nativer Lauf mit AddressSanitizer und UndefinedBehaviorSanitizer. Gültige Meldungstypen, leere/maximale Eingabe, Übergröße, Nullbytes, zu lange und abgeschnittene Zeilen, CRLF/BOM, Fehlercodes sowie 20.000 deterministische Mutationen. Kein gemeldeter Sanitizerfehler. |
| Integration | Zehn Node-Tests gegen das echte WebAssembly-Modul bzw. die Prüfsummenlogik. Geprüft wurden unter anderem getrennte Starts und Dateien, widersprüchliche Netzmeldungen, vorzeichenlose Fehlercodes, unvollständige Startmarken und Ausgabebegrenzung. |
| Browser und Referenzdateien | Chromium 156 führte die echte HTML-Dateiauswahl bei abgeschaltetem Netzwerk aus. Beide Original-PRX korrekt erkannt. Fehlerprotokoll, Nullbytes, Übergröße, leere Datei, Abwahl und unbekannter Hash geprüft. Null externe Seitenanfragen. Die lokale Browser-Verwaltung blockiert direkte Datei-URLs. Deshalb erhielt der Browser die unveränderten HTML-Bytes aus dem Testprogramm. Der Workflow prüft den direkten Datei-Aufruf separat. |
| Hardware | Keine Konsole verbunden. Keine PS5-Firmware, laufende Spielinstanz, Anzeige, HEN-Komponente oder Menüfunktion auf echter Hardware getestet. Für die PS5 liegt auch kein neuer Build vor. |

Die Logbeispiele der Tests sind synthetisch. Die beiden PRX-Prüfungen verwendeten die echten Upstream-Dateien. Die Befunde gelten für den Leser und die identifizierten Artefakte. Compiler-, Sanitizer- oder Browsertests des Lesers liefern keine Kompatibilitätsaussage für die Konsole.
