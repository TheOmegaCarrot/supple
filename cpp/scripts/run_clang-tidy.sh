#!/bin/sh

# Tiny wrapper around clang-tidy

find . -type f -name "*.cpp" | xargs -P0 -I{} clang-tidy {} --quiet --header-filter='.*' 2>/dev/null
