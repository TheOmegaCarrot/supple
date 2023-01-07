cmake_minimum_required(VERSION 3.22.1)

project(supl LANGUAGES CXX)

# CMake options: FULL_TESTS: perform C++20 and C++23 tests as well OMIT_20: Omit
# C++20 tests (for compatibility with older compilers) OMIT_23: Omit C++23 tests
# (for compatibility with older compilers) AUTOMATED_BUILD: Internal flag,
# currently controls exporting of compile commands NO_SANITIZERS: Allows
# disabling sanitizers for debug builds

# Do not export if doing an automated build by cmake-all.sh
if(NOT AUTOMATED_BUILD)
  message("-- Exporting compile commands")
  # compile commands needed for clangd
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # compile_commands needed in source directory
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
  add_compile_options(-diag-enable=all -diag-disable=10441 -diag-disable=13000)
  add_link_options(-diag-enable=all -diag-disable=10441 -diag-disable=13000)
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
  # GCC and Clang are the only compilers I have to test against
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    # Use sanitizers in debug
    message("-- Build type: Debug")
    if(NOT NO_SANITIZERS)
      message("-- Using sanitizers")
      set(SANITIZERS address,leak,undefined)
      add_compile_options(-fsanitize=${SANITIZERS})
      add_link_options(-fsanitize=${SANITIZERS})
    endif()
  elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    message("-- Build type: Release")
  endif()
endif()

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
