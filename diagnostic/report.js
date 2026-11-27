// Shared by the local browser document and the WebAssembly integration tests.
function emptySession() {
    return { records: 0, events: new Set(), network: null, lastNetworkError: null, incomplete: 0 };
}

async function createLogReader(moduleBytes) {
    const keys = new Map();
    const decoder = new TextDecoder();
    let memory;
    let current;
    let starts = 0;

    function observed(pointer, stateCode, reachCode) {
        if (!keys.has(pointer)) {
            const bytes = new Uint8Array(memory.buffer);
            const end = bytes.indexOf(0, pointer);
            if (end < pointer || end - pointer > 64) throw new Error("Ungültiger Ereignisname.");
            keys.set(pointer, decoder.decode(bytes.subarray(pointer, end)));
        }
        const name = keys.get(pointer);
        if (name === "start") {
            starts++;
            current = emptySession();
        }
        if (name === "incomplete") {
            current.incomplete++;
            return;
        }
        current.records++;
        current.events.add(name);
        if (["reported-offline", "online", "network-error"].includes(name)) {
            current.network = { name, stateCode: stateCode >>> 0, reachCode: reachCode >>> 0 };
            if (name === "network-error") current.lastNetworkError = current.network;
        }
    }

    const { instance } = await WebAssembly.instantiate(moduleBytes, { env: { observed } });
    const api = instance.exports;
    memory = api.memory;
    const capacity = api.log_capacity();
    function parse(bytes) {
        if (!(bytes instanceof Uint8Array) || bytes.byteLength > capacity) {
            throw new Error("Als Log wird eine Datei mit höchstens 4 MiB erwartet.");
        }
        current = emptySession();
        starts = 0;
        new Uint8Array(memory.buffer, api.log_input(), bytes.byteLength).set(bytes);
        if (api.inspect_log(bytes.byteLength) !== 0) {
            throw new Error("Die Datei enthält Nullbytes. Erwartet wird ein Textprotokoll aus menu.log.");
        }
        return { starts, ...current };
    }
    return { parse, capacity };
}

const details = [
    ["game-mapping", "Der Menüthread meldet, dass er einen Spiel-Speicherbereich gefunden hat."],
    ["size-mismatch", "Buildprüfung fehlgeschlagen. Die gemeldete Größe des Spielabbilds weicht ab."],
    ["table-unreadable", "Buildprüfung fehlgeschlagen. Die erwartete Tabelle war unlesbar."],
    ["table-mismatch", "Buildprüfung fehlgeschlagen. Der Tabellenvergleich meldet eine Abweichung."],
    ["version-match", "Der interne Tabellenvergleich meldet eine Übereinstimmung. Titel-ID und Paketversion fehlen im Log."],
    ["interface-ok", "Mindestens eine Schnittstelle meldet Erfolg."],
    ["interface-error", "Mindestens eine Schnittstelle meldet einen Fehler. Der Modulstart kann zuvor andere Schnittstellen verändert haben."],
    ["overlay-error", "Die Anzeige meldet, dass kein beschreibbarer Bildpuffer gefunden wurde."],
    ["overlay-no-handle", "Die Anzeige meldet Bildpuffer mit ungültigem Anzeigehandle."],
    ["overlay-reported", "Die Anzeige meldet Puffer und Handle. Ob ein Menü sichtbar war, bleibt offen."],
    ["operation-reported", "Mindestens eine Menüoperation erreichte ihren Protokollpunkt. Ihre Wirkung im Spiel bleibt offen."]
];

function describe(report) {
    const lines = [];
    if (report.starts) {
        lines.push("Erkannte Modulstarts: " + report.starts + ". Ausgewertet wird der letzte begonnene Lauf.");
    } else {
        lines.push(report.records ? "Protokollfragment ohne Startmarke. Die Laufzuordnung ist offen."
                                  : "Keine vollständige, bekannte Diagnosemeldung gefunden.");
    }
    if (report.incomplete) lines.push("Übersprungene überlange oder unvollständige Zeilen im Abschnitt: " + report.incomplete + ".");
    for (const [key, text] of details) {
        if (report.events.has(key)) lines.push(text);
    }
    if (report.events.has("online")) lines.push("Das Menü meldete in diesem Abschnitt mindestens einmal eine erreichbare PSN-Verbindung und gesperrte Funktionen.");
    const hex = value => "0x" + value.toString(16).padStart(8, "0");
    if (report.lastNetworkError) {
        const err = report.lastNetworkError;
        lines.push("Die letzte fehlgeschlagene PSN-Abfrage lieferte " + hex(err.stateCode) +
            " für den Status und " + hex(err.reachCode) +
            " für die Erreichbarkeit. Ein solcher Fehler kann im Original als offline erscheinen.");
    }
    if (report.network) {
        const names = { "reported-offline": "vom Menü als offline gemeldet", online: "vom Menü als online gemeldet", "network-error": "unbestimmt" };
        lines.push("Letzte protokollierte PSN-Abfrage: " + names[report.network.name] +
            ". Rückgabecodes für Status und Erreichbarkeit: " + hex(report.network.stateCode) + " / " +
            hex(report.network.reachCode) + ".");
    }
    lines.push("Firmware, exakte Spielversion, Loader-Version und Story-Modus sind aus diesen Meldungen nicht bestimmbar.");
    return lines.join("\n\n");
}

async function inspectAsset(bytes, catalog) {
    const digest = await crypto.subtle.digest("SHA-256", bytes);
    const hash = Array.from(new Uint8Array(digest), b => b.toString(16).padStart(2, "0")).join("");
    const release = catalog.find(item => item.sha256 === hash && item.bytes === bytes.byteLength);
    return { hash, tag: release ? release.tag : null };
}

if (typeof module !== "undefined" && module.exports) {
    module.exports = { createLogReader, describe, inspectAsset };
}
