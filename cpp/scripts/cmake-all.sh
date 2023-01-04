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
  PREFIX=${1}
  COMPILER=${2}
  shift
  shift
  FLAGS=${@}

  echo ${PREFIX}
  echo ${COMPILER}
  echo ${FLAGS}

  if [ -x ${PREFIX}/${COMPILER} ]; then

    cat << EOF

##############################################################################
                              ${COMPILER} DEBUG                                     
##############################################################################

EOF

    cmake -S cpp -B all/${COMPILER}-debug -DCMAKE_BUILD_TYPE=Debug -DFULL_TESTS=YES ${FLAGS} -DNO_SANITIZERS=YES -DCMAKE_CXX_COMPILER=${PREFIX}/${COMPILER} -DAUTOMATED_BUILD=YES -G Ninja 
    echo
    cmake --build all/${COMPILER}-debug --parallel $(nproc)
    cmake --build all/${COMPILER}-debug --parallel $(nproc) --target test

    cat << EOF

##############################################################################
                               ${COMPILER} RELEASE                                   
##############################################################################

EOF


    cmake -S cpp -B all/${COMPILER}-release -DCMAKE_BUILD_TYPE=Release -DFULL_TESTS=Yes ${FLAGS} -DCMAKE_CXX_COMPILER=${PREFIX}/${COMPILER} -DAUTOMATED_BUILD=YES -G Ninja 
    echo
    cmake --build all/${COMPILER}-release --parallel $(nproc)
    cmake --build all/${COMPILER}-release --parallel $(nproc) --target test

  else

    echo "${PREFIX}/${COMPILER}" not found

  fi

}

test_compiler /usr/bin g++-9 -DOMIT_23=YES
test_compiler /usr/bin g++-10 -DOMIT_23=YES
test_compiler /usr/bin g++-11
test_compiler /usr/bin clang++-11 -DOMIT_23=YES
test_compiler /usr/bin clang++-12
test_compiler /usr/bin clang++-13
test_compiler /usr/bin clang++-14
test_compiler /opt/llvm-15/bin clang++
test_compiler /opt/intel-2023/oneapi/compiler/2023.0.0/linux/bin/intel64 icc -DOMIT_23=YES
