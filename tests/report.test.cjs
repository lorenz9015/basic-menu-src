const test = require("node:test");
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const { createLogReader, describe, inspectAsset } = require("../diagnostic/report.js");
const catalog = require("../data/releases.json");
const wasm = fs.readFileSync(path.join(__dirname, "../build/log.wasm"));
const reader = createLogReader(wasm);
const stamp = message => "[     1.020] " + message + "\n";
const header = "Basic Menu, built Nov 26 2026 18:22:43";
const encode = s => new TextEncoder().encode(s);

test("standalone module imports only the event callback", () => {
    assert.deepEqual(WebAssembly.Module.imports(new WebAssembly.Module(wasm)),
        [{ module: "env", name: "observed", kind: "function" }]);
});
test("latest start clears old failures, actions, codes and partial lines", async () => {
    const log = [header, "cheat table unreadable", "ran op 17 arg 1",
        "NP state -1 (ret 0xff), reachability -1 (ret 0x3) -> offline, features allowed", header, "eboot f-10"];
    const r = (await reader).parse(encode(log.map(stamp).join("")));
    assert.equal(r.starts, 2);
    assert.equal(r.records, 2);
    assert.deepEqual([...r.events], ["start","game-mapping"]);
    assert.equal(r.network, null);
});
test("reusing reader never mixes two files", async () => {
    const parser = await reader;
    const old = parser.parse(encode(stamp(header)));
    const fresh = parser.parse(encode(stamp("cheat table unreadable")));
    assert.equal(fresh.starts, 0);
    assert.match(describe(fresh), /Protokollfragment/);
    assert.deepEqual([...old.events], ["start"]);
});
test("failed NP return codes override upstream offline message and retain high bit", async () => {
    const r = (await reader).parse(encode(stamp("NP state 0 (ret 0xffffffff), reachability 1 (ret 0x80000001) -> offline, features allowed")));
    assert.deepEqual(r.network, {name:"network-error", stateCode:4294967295, reachCode:2147483649});
    assert.match(describe(r), /unbestimmt.*0xffffffff \/ 0x80000001/);
    assert.ok(!r.events.has("reported-offline"));
});
test("history keeps intermittent errors and online state after a later offline message", async () => {
    const log = ["NP state -1 (ret 0x1), reachability 0 (ret 0x0) -> offline, features allowed",
        "NP state 1 (ret 0x0), reachability 1 (ret 0x0) -> ONLINE, features locked",
        "NP state 0 (ret 0x0), reachability 0 (ret 0x0) -> offline, features allowed"];
    const r = (await reader).parse(encode(log.map(stamp).join("")));
    assert.equal(r.network.name, "reported-offline");
    assert.ok(r.events.has("online") && r.events.has("network-error"));
    assert.equal(r.lastNetworkError.stateCode, 1);
    assert.match(describe(r), /0x00000001/);
});
test("an incomplete or malformed build marker cannot hide an earlier failure", async () => {
    const entries = ["Basic Menu, built Mar  7 2026 02:03:04", "cheat table unreadable",
        "Basic Menu, built nonsense", "Basic Menu, built Nov 26 2026 24:00:00"];
    const r = (await reader).parse(encode(entries.map(stamp).join("")));
    assert.equal(r.starts, 1);
    assert.ok(r.events.has("table-unreadable"));
});
test("partial and overlong lines do not invent successes", async () => {
    const r = (await reader).parse(encode(stamp("x".repeat(2000)) + stamp("hook fake: ok").trimEnd()));
    assert.equal(r.incomplete, 2);
    assert.equal(r.records, 0);
    assert.match(describe(r), /Keine vollständige/);
});
test("binary and oversized input fail before producing a report", async () => {
    const p = await reader;
    assert.equal(p.capacity, 4194304);
    assert.throws(() => p.parse(encode(stamp(header) + "\0")), /Nullbytes/);
    assert.throws(() => p.parse(new Uint8Array(p.capacity + 1)), /4 MiB/);
    assert.equal(p.parse(new Uint8Array()).records, 0);
});
test("addresses, hook names and arbitrary input are absent from the rendered report", async () => {
    const r = (await reader).parse(encode(["eboot deadbeef-cafefeed","hook PRIVATE: ok","<img onerror=alert(1)>"].map(stamp).join("")));
    assert.doesNotMatch(describe(r), /deadbeef|cafefeed|PRIVATE|<img/);
    assert.ok(r.events.has("interface-ok") && r.events.has("game-mapping"));
});
test("digest comparison requires both exact size and hash", async () => {
    const bytes = encode("abc");
    const hash = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
    assert.deepEqual(await inspectAsset(bytes, catalog), {hash, tag:null});
    assert.deepEqual(await inspectAsset(bytes, [{sha256:hash, bytes:3, tag:"fixture"}]), {hash, tag:"fixture"});
    assert.equal((await inspectAsset(bytes, [{sha256:hash, bytes:4, tag:"fixture"}])).tag, null);
});
