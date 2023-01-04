#!/bin/bash

# Compile and test with GCC and Clang
# in both debug and release configuration
# building against C++17, C++20, and C++23
#
# Library consumers should disregard this file,
# unless you want to compile and run 12 versions
# of this library's tests

cd ..

cat << EOF

##############################################################################
#                               GCC DEBUG                                    #
##############################################################################

EOF

cmake -S cpp -B all/gcc-debug -DCMAKE_BUILD_TYPE=Debug -DFULL_TESTS=YES -DCMAKE_CXX_COMPILER=g++ -DAUTOMATED_BUILD=YES -G Ninja 
echo
cmake --build all/gcc-debug --parallel $(nproc)
cmake --build all/gcc-debug --parallel $(nproc) --target test

cat << EOF

##############################################################################
#                              GCC RELEASE                                   #
##############################################################################

EOF


cmake -S cpp -B all/gcc-release -DCMAKE_BUILD_TYPE=Release -DFULL_TESTS=Yes -DCMAKE_CXX_COMPILER=g++ -DAUTOMATED_BUILD=YES -G Ninja 
echo
cmake --build all/gcc-release --parallel $(nproc)
cmake --build all/gcc-release --parallel $(nproc) --target test


cat << EOF

##############################################################################
#                              CLANG DEBUG                                   #
##############################################################################

EOF

cmake -S cpp -B all/clang-debug -DCMAKE_BUILD_TYPE=Debug -DFULL_TESTS=Yes -DCMAKE_CXX_COMPILER=clang++ -DAUTOMATED_BUILD=YES -G Ninja 
echo
cmake --build all/clang-debug --parallel $(nproc)
cmake --build all/clang-debug --parallel $(nproc) --target test

cat << EOF

##############################################################################
#                             CLANG RELEASE                                  #
##############################################################################

EOF

cmake -S cpp -B all/clang-release -DCMAKE_BUILD_TYPE=Release -DFULL_TESTS=Yes -DCMAKE_CXX_COMPILER=clang++ -DAUTOMATED_BUILD=YES -G Ninja 
echo
cmake --build all/clang-release --parallel $(nproc)
cmake --build all/clang-release --parallel $(nproc) --target test

ICC="/opt/intel/oneapi/compiler/2023.0.0/linux/bin/intel64/icc"

if [ -x ${ICC} ]; then

cat << EOF

##############################################################################
#                              INTEL DEBUG                                   #
##############################################################################

EOF

  cmake -S cpp -B all/intel-debug -DCMAKE_BUILD_TYPE=Debug -DFULL_TESTS=Yes -DCMAKE_CXX_COMPILER=${ICC} -DAUTOMATED_BUILD=YES -G Ninja
  echo
  cmake --build all/intel-debug --parallel $(nproc)
  cmake --build all/intel-debug --parallel $(nproc) --target test

cat << EOF

##############################################################################
#                             INTEL RELEASE                                  #
##############################################################################

EOF

  cmake -S cpp -B all/intel-release -DCMAKE_BUILD_TYPE=Release -DFULL_TESTS=Yes -DCMAKE_CXX_COMPILER=${ICC} -DAUTOMATED_BUILD=YES -G Ninja
  echo
  cmake --build all/intel-release --parallel $(nproc)
  cmake --build all/intel-release --parallel $(nproc) --target test

fi
