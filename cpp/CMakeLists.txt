cmake_minimum_required(VERSION 3.16.0)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  message("-- Build type not specified")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

project(supl LANGUAGES CXX)

if(AUTOMATED_BUILD)
  set(LINK_COMPILE_COMMANDS NO)
endif()

option(LINK_COMPILE_COMMANDS "Cmake should link compile commands into source directory" YES)
option(SANITIZE_DEBUG "Sanitizers should be used in debug builds" YES)
option(SANITIZE_RELEASE "Sanitizers should be used in release builds" NO)
option(FULL_TESTS "Test C++20 and 23 as well" NO)
option(OMIT_20 "C++20 tests should be omitted - ignored if FULL_TESTS is false" NO)
option(OMIT_23 "C++23 tests should be omitted - ignored if FULL_TESTS is false" NO)

# Options specific to compiling with clang
option(DO_CLANG_TIDY "Run clang-tidy during build" NO)
option(DO_TIME_TRACE "Run time trace during build" YES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(LINK_COMPILE_COMMANDS)
  # compile_commands needed in source directory
  message("-- Linking compile_commands.json")
  file(CREATE_LINK "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
       "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json" COPY_ON_ERROR
       SYMBOLIC)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # set warnings for GCC
  message("-- Using GCC")
  add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Wcast-align
    -Wcast-qual
    -Wredundant-decls
    -Wconversion
    -Wnon-virtual-dtor
    -Wunused
    -Wdouble-promotion
    -Wformat=2
    -Wnull-dereference
    -Wimplicit-fallthrough
    -Wuninitialized
    -Wstrict-overflow=2
    -Wold-style-cast
    -Wfloat-equal
    -Wundef
    -Wshadow
    -Wfree-nonheap-object
    -Wsign-conversion
    -Wno-multichar
    -Woverloaded-virtual
    -Wctor-dtor-privacy
    -Wsign-promo
    -Wmisleading-indentation
    -Wzero-as-null-pointer-constant
    -Wsuggest-override
    -Wno-return-type)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # set warnings for Clang
  message("-- Using Clang")
  add_compile_options(
    -Weverything -Wno-ctad-maybe-unsupported -Wno-missing-prototypes
    -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)

  # Use clang-tidy if not an automated build by cmake-all.sh
  if(DO_CLANG_TIDY)
    message("-- Using clang-tidy")
    set(CMAKE_CXX_CLANG_TIDY
        "clang-tidy;--config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy")
  endif()

  if(DO_TIME_TRACE)
    message("-- Running time trace")
    add_compile_options(-ftime-trace)
  endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  message("-- Using Intel")
  add_compile_options(-diag-enable=all -diag-disable=10441 -diag-disable=13000 -diag-disable=11074 -diag-disable=11076)
  add_link_options(-diag-enable=all -diag-disable=10441 -diag-disable=13000 -diag-disable=11074 -diag-disable=11076)
  set(OMIT_23 YES)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
  message("-- Using Intel LLVM")
  add_compile_options(-diag-enable=all -Rno-debug-disables-optimization)
  add_link_options(-diag-enable=all -Rno-debug-disables-optimization)
  set(OMIT_23 YES)

elseif(MSVC)
  # I do not have access to MSVC, and thus cannot test it
  message("-- Using MSVC")
  message("-- WARNING: Use of MSVC is UNSUPPORTED and UNTESTED")
  message("-- WARNING: Use of MSVC is UNSUPPORTED and UNTESTED")
  message("-- WARNING: Use of MSVC is UNSUPPORTED and UNTESTED")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                           "Clang")

  # GCC and Clang are the only compilers I have which support sanitizers
  if(SANITIZE_DEBUG AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("-- Using sanitizers in debug")
    set(SANITIZERS address,leak,undefined)
    add_compile_options(-fsanitize=${SANITIZERS})
    add_link_options(-fsanitize=${SANITIZERS})
  endif()

  if(SANITIZE_RELEASE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    message("-- Using sanitizers in release")
    set(SANITIZERS address,leak,undefined)
    add_compile_options(-fsanitize=${SANITIZERS})
    add_link_options(-fsanitize=${SANITIZERS})
  endif()

endif()

message("-- Build type: ${CMAKE_BUILD_TYPE}")

# find supple package
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/..)
find_package(supple REQUIRED)

# find all test source files
file(GLOB_RECURSE core_test_files
     ${CMAKE_CURRENT_SOURCE_DIR}/tst/src/core/*.cpp)

enable_testing()

# make ctest give useful output to console on test failure
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

function(test_for numeric_standard)
  foreach(test_file ${core_test_files})
    # remove parent directory path and extension
    get_filename_component(filename ${test_file} NAME_WLE)

    set(test_exe_name core_${filename}_${numeric_standard})
    add_executable(${test_exe_name} ${test_file})
    target_link_libraries(${test_exe_name} supple::full)
    target_compile_features(${test_exe_name} PUBLIC cxx_std_${numeric_standard})

    # remove test_ prefix for prettier test output
    string(REPLACE "test_" "" test_name ${test_exe_name})
    add_test(${test_name} ${test_exe_name})
  endforeach()
endfunction()

message("-- Testing C++17")
test_for(17)
if(FULL_TESTS)
  if(NOT OMIT_20)
    message("-- Testing C++20")
    test_for(20)
  endif()
  if(NOT OMIT_23)
    message("-- Testing C++23")
    test_for(23)
  endif()
endif()
