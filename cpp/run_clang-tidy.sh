#!/bin/sh

# Tiny wrapper around clang-tidy
find . -type f -name "*.cpp" | xargs -P0 -I{} clang-tidy {} --quiet --header-filter='.*' -- -xc++ -std=c++17 -Iinc/core -Itst/inc -Itst/inc/core 2>/dev/null
