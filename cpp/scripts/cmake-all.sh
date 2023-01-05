#!/bin/bash

# Compile and test with GCC and Clang
# in both debug and release configuration
# building against C++17, C++20, and C++23
#
# Library consumers should disregard this file,
# unless you want to compile and run 12 versions
# of this library's tests

cd ..

test_compiler() {
  COMPILER=${1}
  NAME=${2}
  shift
  shift
  FLAGS=${@}

  if [ -x ${PREFIX}/${COMPILER} ]; then

    cat << EOF

##############################################################################
                              ${NAME} DEBUG                                     
##############################################################################

EOF

    cmake -S cpp -B all/${NAME}-debug -DCMAKE_BUILD_TYPE=Debug -DFULL_TESTS=YES ${FLAGS} -DNO_SANITIZERS=YES -DCMAKE_CXX_COMPILER=${COMPILER} -DAUTOMATED_BUILD=YES -G Ninja 
    echo
    cmake --build all/${NAME}-debug --parallel $(nproc)
    cmake --build all/${NAME}-debug --parallel $(nproc) --target test

    cat << EOF

##############################################################################
                               ${NAME} RELEASE                                   
##############################################################################

EOF


    cmake -S cpp -B all/${NAME}-release -DCMAKE_BUILD_TYPE=Release -DFULL_TESTS=Yes ${FLAGS} -DCMAKE_CXX_COMPILER=${COMPILER} -DAUTOMATED_BUILD=YES -G Ninja 
    echo
    cmake --build all/${NAME}-release --parallel $(nproc)
    cmake --build all/${NAME}-release --parallel $(nproc) --target test

  else

    echo "${PREFIX}/${COMPILER}" not found

  fi

}

test_compiler /usr/bin/g++-9 GCC-9 -DOMIT_23=YES
test_compiler /usr/bin/g++-10 GCC-10 -DOMIT_23=YES
test_compiler /usr/bin/g++-11 GCC-11
test_compiler /opt/gcc-12/bin/g++ GCC-12
# test_compiler /opt/gcc-trunk/bin/g++ GCC-TRUNK
test_compiler /usr/bin/clang++-11 CLANG-11 -DOMIT_23=YES
test_compiler /usr/bin/clang++-12 CLANG-12
test_compiler /usr/bin/clang++-13 CLANG-13
test_compiler /usr/bin/clang++-14 CLANG-14
test_compiler /opt/llvm-15/bin/clang++ CLANG-15
test_compiler /opt/llvm-trunk/bin clang++ CLANG-TRUNK
test_compiler /opt/intel-2023/oneapi/compiler/2023.0.0/linux/bin/intel64/icc ICC -DOMIT_23=YES
test_compiler /opt/intel-2023/oneapi/compiler/2023.0.0/linux/bin/icpx ICPX -DOMIT_23=YES
