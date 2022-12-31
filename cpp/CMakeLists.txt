cmake_minimum_required(VERSION 3.22.1)

project(supl LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(CREATE_LINK "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
     "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json" COPY_ON_ERROR SYMBOLIC)

macro(sanitize)
  message("-- Using sanitizers")
  set(SANITIZERS address,leak,undefined)
  add_compile_options(-fsanitize=${SANITIZERS})
  add_link_options(-fsanitize=${SANITIZERS})
endmacro()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
  message("-- Using Clang")
  add_compile_options(
    -Weverything -Wno-ctad-maybe-unsupported -Wno-missing-prototypes
    -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)

elseif(MSVC)
  message("-- Using MSVC")
  message("-- WARNING: Use of MSVC is UNSUPPORTED and UNTESTED")
  message("-- WARNING: Use of MSVC is UNSUPPORTED and UNTESTED")
  message("-- WARNING: Use of MSVC is UNSUPPORTED and UNTESTED")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                           "Clang")
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    message("-- Build type: Debug")
    sanitize()
  elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    message("-- Build type: Release")
  endif()
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/..)
find_package(supple REQUIRED)

# Test executables

file(GLOB_RECURSE test_files ${CMAKE_CURRENT_SOURCE_DIR}/tst/src/*.cpp)

add_executable(run_tests_17 ${source_files} ${test_files})
target_link_libraries(run_tests_17 supple::full)
target_include_directories(run_tests_17 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc)
target_include_directories(run_tests_17 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc/core)
target_compile_features(run_tests_17 PUBLIC cxx_std_17)

add_executable(run_tests_20 ${source_files} ${test_files})
target_link_libraries(run_tests_20 supple::full)
target_include_directories(run_tests_20 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc)
target_include_directories(run_tests_20 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc/core)
target_compile_features(run_tests_20 PUBLIC cxx_std_20)

add_executable(run_tests_23 ${source_files} ${test_files})
target_link_libraries(run_tests_23 supple::full)
target_include_directories(run_tests_23 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc)
target_include_directories(run_tests_23 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc/core)
target_compile_features(run_tests_23 PUBLIC cxx_std_23)
