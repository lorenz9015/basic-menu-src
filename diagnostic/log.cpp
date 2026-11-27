// Offline reader for the diagnostic messages emitted by Basic Menu.
// It accepts a bounded byte array. The only host callback reports fixed event
// names and numeric error codes. Raw input is never passed to the callback.
#include <stddef.h>
#include <stdint.h>

#ifdef __wasm__
extern "C" __attribute__((import_module("env"), import_name("observed")))
void observed(const char* event, uint32_t state_code, uint32_t reach_code);
#else
extern "C" void observed(const char* event, uint32_t state_code, uint32_t reach_code);
#endif

namespace {
constexpr size_t max_file = 4u * 1024 * 1024;
constexpr size_t max_line = 1024;
unsigned char input[max_file];

struct Cursor {
    const unsigned char* pos;
    const unsigned char* end;

    bool take(const char* literal) {
        auto next = pos;
        for (; *literal; ++literal) {
            if (next == end || *next != static_cast<unsigned char>(*literal)) return false;
            ++next;
        }
        pos = next;
        return true;
    }

    bool rest_is(const char* text) const {
        auto copy = *this;
        return copy.take(text) && copy.pos == end;
    }

    // All numbers in the log come from bounded printf integer formats.
    bool number(unsigned base, unsigned max_digits, uint64_t* value = nullptr) {
        auto next = pos;
        uint64_t result = 0;
        unsigned count = 0;
        while (next != end) {
            const auto c = *next;
            unsigned digit;
            if (c >= '0' && c <= '9') digit = c - '0';
            else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10u;
            else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10u;
            else break;
            if (digit >= base) break;
            if (++count > max_digits) return false;
            result = result * base + digit;
            ++next;
        }
        if (!count) return false;
        if (value) *value = result;
        pos = next;
        return true;
    }

    bool decimal() {
        take("-");
        return number(10, 10);
    }
};

bool build_stamp(Cursor c) {
    constexpr const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    bool month_found = false;
    for (auto month : months) {
        if (c.take(month)) { month_found = true; break; }
    }
    if (!month_found || !c.take(" ") || c.end - c.pos != 16) return false;
    const auto* p = c.pos;
    auto digit = [](unsigned char b) { return b >= '0' && b <= '9'; };
    auto pair = [&](size_t at) { return (p[at] - '0') * 10 + (p[at + 1] - '0'); };
    if (p[2] != ' ' || p[7] != ' ' || p[10] != ':' || p[13] != ':') return false;
    for (size_t i = 0; i < 16; ++i) {
        if (i == 2 || i == 7 || i == 10 || i == 13 || (i == 0 && p[i] == ' ')) continue;
        if (!digit(p[i])) return false;
    }
    const int day = p[0] == ' ' ? p[1] - '0' : pair(0);
    return day >= 1 && day <= 31 && pair(8) < 24 && pair(11) < 60 && pair(14) < 60;
}

void network(Cursor c) {
    uint64_t state = 0, reach = 0;
    if (!c.decimal() || !c.take(" (ret 0x") || !c.number(16, 8, &state) ||
        !c.take("), reachability ") || !c.decimal() || !c.take(" (ret 0x") ||
        !c.number(16, 8, &reach) || !c.take(") -> ")) return;
    const bool online = c.rest_is("ONLINE, features locked");
    if (!online && !c.rest_is("offline, features allowed")) return;
    // A failed query can produce "offline, features allowed" upstream.
    observed((state || reach) ? "network-error" : online ? "online" : "reported-offline",
             static_cast<uint32_t>(state), static_cast<uint32_t>(reach));
}

void interface_status(Cursor c) {
    // Hook names are implementation details. Do not copy them to the report.
    const auto start = c.pos;
    while (c.pos != c.end && *c.pos != ':') ++c.pos;
    if (c.pos == start || c.pos - start > 64 || !c.take(": ")) return;
    if (c.rest_is("ok")) observed("interface-ok", 0, 0);
    else if (c.rest_is("slot not readable") || c.rest_is("slot empty") ||
             c.rest_is("mprotect failed") ||
             (c.take("slot holds 0x") && c.number(16, 16) && c.rest_is(", not an import"))) {
        observed("interface-error", 0, 0);
    }
}

void classify(Cursor c) {
    if (!c.take("[")) return;
    while (c.take(" ")) {}
    if (!c.number(10, 19) || !c.take(".") || c.end - c.pos < 3) return;
    for (unsigned n = 0; n < 3; ++n) {
        if (*c.pos < '0' || *c.pos > '9') return;
        ++c.pos;
    }
    if (!c.take("] ")) return;

    if (c.take("Basic Menu, built ")) {
        if (build_stamp(c)) observed("start", 0, 0);
    } else if (c.take("eboot size 0x")) {
        if (c.number(16, 16) && c.take(" != expected 0x") && c.number(16, 16) &&
            c.rest_is(" - features off")) observed("size-mismatch", 0, 0);
    } else if (c.take("eboot ")) {
        if (c.number(16, 16) && c.take("-") && c.number(16, 16) && c.pos == c.end) {
            observed("game-mapping", 0, 0);
        }
    } else if (c.rest_is("cheat table unreadable")) {
        observed("table-unreadable", 0, 0);
    } else if (c.take("cheat table [2]=")) {
        if (!c.number(16, 16) || !c.take(" [3]=") || !c.number(16, 16) ||
            !c.take(" [4]=") || !c.number(16, 16) || !c.take(" [6]=") ||
            !c.number(16, 16) || !c.take(" -> ")) return;
        if (c.rest_is("version OK")) observed("version-match", 0, 0);
        else if (c.rest_is("MISMATCH, features off")) observed("table-mismatch", 0, 0);
    } else if (c.take("NP state ")) {
        network(c);
    } else if (c.take("hook ")) {
        interface_status(c);
    } else if (c.rest_is("overlay: no writable display buffer")) {
        observed("overlay-error", 0, 0);
    } else if (c.take("overlay: buffers ")) {
        if (!c.number(16, 16) || !c.take(" ") || !c.number(16, 16) || !c.take(", handle ")) return;
        const bool negative = c.take("-");
        if (c.number(10, 10) && c.pos == c.end) {
            observed(negative ? "overlay-no-handle" : "overlay-reported", 0, 0);
        }
    } else if (c.take("ran op ")) {
        if (c.decimal() && c.take(" arg ")) {
            c.take("-");
            if (c.number(10, 19) && c.pos == c.end) observed("operation-reported", 0, 0);
        }
    }
}
} // namespace

extern "C" unsigned char* log_input() { return input; }
extern "C" uint32_t log_capacity() { return sizeof(input); }

// Return 1 for an oversized input, 2 for binary input. An error emits no events.
extern "C" int inspect_log(uint32_t length) {
    if (length > sizeof(input)) return 1;
    for (uint32_t i = 0; i < length; ++i) {
        if (input[i] == 0) return 2;
    }
    size_t start = 0;
    if (length >= 3 && input[0] == 0xef && input[1] == 0xbb && input[2] == 0xbf) start = 3;
    for (size_t i = start; i < length; ++i) {
        if (input[i] != '\n') continue;
        const size_t end = (i > start && input[i - 1] == '\r') ? i - 1 : i;
        if (end - start > max_line) observed("incomplete", 0, 0);
        else classify({input + start, input + end});
        start = i + 1;
    }
    // A log may have been copied during a write. Never infer success from a partial line.
    if (start != length) observed("incomplete", 0, 0);
    return 0;
}
