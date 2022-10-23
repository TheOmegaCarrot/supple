cmake_minimum_required(VERSION 3.22.1)

project(utils LANGUAGES CXX)
set(standard 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

macro(sanitize)
  message("-- Using sanitizers")
  set(SANITIZERS address,leak,undefined)
  add_compile_options(-fsanitize=${SANITIZERS})
  add_link_options(-fsanitize=${SANITIZERS})
endmacro()

macro(optimize opt)
  message("-- Using -O${opt}")
  add_compile_options(-O${opt})
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
    -Weffc++
    -Wno-return-type)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message("-- Using Clang")
  add_compile_options(
    -Weverything -Wno-ctad-maybe-unsupported -Wno-missing-prototypes
    -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)

elseif(MSVC)
  message("-- Using MSVC")
  # Untested, unsupported
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    message("-- Build type: Debug")
    optimize(0)
    sanitize()
    add_compile_options(-g)
  elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    message("-- Build type: Release")
    optimize(3)
  endif()
endif()

include_directories(inc)

# Main executable
file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${source_files})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${standard})

# Test executable
set(test_exe_name run_tests)
file(GLOB_RECURSE test_files ${CMAKE_CURRENT_SOURCE_DIR}/tst/src/*.cpp)
add_executable(${test_exe_name} ${source_files} ${test_files})
target_include_directories(${test_exe_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tst/inc)
set_property(TARGET ${test_exe_name} PROPERTY CXX_STANDARD ${standard})
