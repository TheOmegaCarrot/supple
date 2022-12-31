#!/bin/sh

# Tiny wrapper around clang-tidy

cat << EOF

##############################################################################
#                                C++ 17                                      #
##############################################################################

EOF

find . -type f -name "*.cpp" | xargs -P0 -I{} clang-tidy {} --quiet --header-filter='.*' -- -xc++ -std=c++17 -Iinc/core -Itst/inc -Itst/inc/core 2>/dev/null

cat << EOF

##############################################################################
#                                C++ 20                                      #
##############################################################################

EOF

find . -type f -name "*.cpp" | xargs -P0 -I{} clang-tidy {} --quiet --header-filter='.*' -- -xc++ -std=c++20 -Iinc/core -Itst/inc -Itst/inc/core 2>/dev/null

#cat << EOF

###############################################################################
##                                C++ 23                                      #
###############################################################################

#EOF

#find . -type f -name "*.cpp" | xargs -P0 -I{} clang-tidy {} --quiet --header-filter='.*' -- -xc++ -std=c++2b -Iinc/core -Itst/inc -Itst/inc/core 2>/dev/null
