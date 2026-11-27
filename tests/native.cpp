#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <string>
#include <vector>

extern "C" unsigned char* log_input();
extern "C" uint32_t log_capacity();
extern "C" int inspect_log(uint32_t);

struct Event { std::string name; uint32_t first, second; };
static std::vector<Event> events;
extern "C" void observed(const char* key, uint32_t a, uint32_t b) {
    events.push_back({key, a, b});
}

static int parse(const std::string& text) {
    assert(text.size() <= log_capacity());
    events.clear();
    std::memcpy(log_input(), text.data(), text.size());
    return inspect_log(static_cast<uint32_t>(text.size()));
}

static std::string line(const std::string& body) { return "[   120.009] " + body + "\n"; }

int main() {
    const std::vector<std::pair<std::string, std::string>> cases{
        {"Basic Menu, built Nov 26 2026 18:22:43", "start"},
        {"eboot 10-1a", "game-mapping"},
        {"eboot size 0x10 != expected 0xab - features off", "size-mismatch"},
        {"cheat table unreadable", "table-unreadable"},
        {"cheat table [2]=1 [3]=2 [4]=3 [6]=4 -> version OK", "version-match"},
        {"cheat table [2]=1 [3]=2 [4]=3 [6]=4 -> MISMATCH, features off", "table-mismatch"},
        {"hook fake: ok", "interface-ok"},
        {"hook fake: slot not readable", "interface-error"},
        {"hook fake: slot empty", "interface-error"},
        {"hook fake: slot holds 0xfa, not an import", "interface-error"},
        {"hook fake: mprotect failed", "interface-error"},
        {"overlay: no writable display buffer", "overlay-error"},
        {"overlay: buffers 1 2, handle 12", "overlay-reported"},
        {"overlay: buffers 1 2, handle -1", "overlay-no-handle"},
        {"ran op 2 arg -9223372036854775808", "operation-reported"},
        {"NP state 1 (ret 0x0), reachability 1 (ret 0x0) -> ONLINE, features locked", "online"},
        {"NP state 0 (ret 0x0), reachability -1 (ret 0x0) -> offline, features allowed", "reported-offline"},
        {"NP state -1 (ret 0xFFFFFFFD), reachability 0 (ret 0x80000001) -> offline, features allowed", "network-error"}
    };
    for (const auto& [message, expected] : cases) {
        assert(parse(line(message)) == 0);
        assert(events.size() == 1 && events[0].name == expected);
    }
    assert(events[0].first == 0xfffffffdu && events[0].second == 0x80000001u);
    const auto stamp = line(cases.front().first);
    assert(parse(std::string("\xef\xbb\xbf") + stamp.substr(0, stamp.size()-1) + "\r\n") == 0);
    assert(events.size() == 1 && events[0].name == "start");
    assert(parse(stamp.substr(0, stamp.size()-1)) == 0 && events[0].name == "incomplete");
    assert(parse(std::string(1025, 'x') + "\n" + stamp) == 0 && events.size() == 2);
    assert(parse(stamp + std::string(1, '\0')) == 2 && events.empty());
    assert(parse("") == 0 && events.empty());
    assert(inspect_log(UINT32_MAX) == 1 && events.empty());
    assert(parse(std::string(log_capacity(), 'x')) == 0 && events.size() == 1);
    for (const char* invalid : {"[ 1.00] cheat table unreadable\n", "[ 1.0044] cheat table unreadable\n",
        "[ 1.004] cheat table unreadable suffix\n", "version OK\n", "[ 1.004] hook : ok\n",
        "[ 1.004] eboot 1234567890abcdef1-2\n", "[ 1.004] ran op 2 arg \n",
        "[ 1.004] NP state 0 (ret 0x100000000), reachability 0 (ret 0x0) -> offline, features allowed\n"}) {
        assert(parse(invalid) == 0 && events.empty());
    }
    std::mt19937 rng(1360);
    for (int i = 0; i != 20000; ++i) {
        std::string data = line(cases[rng() % cases.size()].first);
        for (unsigned n = 0, edits = 1 + rng()%5; n < edits; ++n) {
            const size_t where = rng() % (data.size() + 1);
            switch (rng()%3) {
                case 0: data.resize(where); break;
                case 1: data.insert(where, 1, static_cast<char>(rng()%256)); break;
                default: if (where < data.size()) data[where] = static_cast<char>(rng()%256);
            }
        }
        const int result = parse(data);
        assert(result == (data.find('\0') == std::string::npos ? 0 : 2));
        if (result) assert(events.empty());
    }
    std::puts("Native log reader: protocol cases, size boundaries and 20000 deterministic mutations passed.");
}
