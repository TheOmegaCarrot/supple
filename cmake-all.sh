#!/bin/bash

cmake -S cpp -B all/gcc-debug -DCMAKE_BUILD_TYPE=Debug
cmake -S cpp -B all/gcc-release -DCMAKE_BUILD_TYPE=Release
env CXX=/usr/bin/clang++ cmake -S cpp -B all/clang-debug -DCMAKE_BUILD_TYPE=Debug
env CXX=/usr/bin/clang++ cmake -S cpp -B all/clang-release -DCMAKE_BUILD_TYPE=Release


cat << EOF

##############################################################################
#                               GCC DEBUG                                    #
##############################################################################

EOF

cmake --build all/gcc-debug --parallel $(nproc)
./all/gcc-debug/run_tests_17
./all/gcc-debug/run_tests_20
./all/gcc-debug/run_tests_23



cat << EOF

##############################################################################
#                              GCC RELEASE                                   #
##############################################################################

EOF


cmake --build all/gcc-release --parallel $(nproc)
./all/gcc-release/run_tests_17
./all/gcc-release/run_tests_20
./all/gcc-release/run_tests_23



cat << EOF

##############################################################################
#                              CLANG DEBUG                                   #
##############################################################################

EOF


cmake --build all/clang-debug --parallel $(nproc)
./all/clang-debug/run_tests_17
./all/clang-debug/run_tests_20
./all/clang-debug/run_tests_23


cat << EOF

##############################################################################
#                             CLANG RELEASE                                  #
##############################################################################

EOF


cmake --build all/clang-release --parallel $(nproc)
./all/clang-release/run_tests_17
./all/clang-release/run_tests_20
./all/clang-release/run_tests_23

