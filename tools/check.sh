#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
python3 tools/build.py

# Separate compilation runs GCC's analyzer passes before the sanitizer run.
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -fanalyzer -c diagnostic/log.cpp -o build/analysis.o
"${WASM_CXX:-clang++-19}" --analyze -std=c++17 -Xanalyzer -analyzer-werror -Xanalyzer -analyzer-output=text diagnostic/log.cpp
g++ -std=c++17 -O1 -g -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined \
    -fno-sanitize-recover=all -fno-omit-frame-pointer diagnostic/log.cpp tests/native.cpp -o build/log-test
./build/log-test
node --check diagnostic/report.js
node --test tests/report.test.cjs
python3 tests/browser.py "$@"
