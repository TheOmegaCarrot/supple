cmake_minimum_required(VERSION 3.16.0)

project(
  supple
  VERSION 0.7.0
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(supple_message)

set(SUPPLE_MAIN_PROJECT NO)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(SUPPLE_MAIN_PROJECT YES)
  supple_status_message("Supple is the main project")
endif()

set(SUPPLE_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

option(SUPPLE_STATUS "Supple should provide status messages"
       ${SUPPLE_MAIN_PROJECT})

option(SUPPLE_SANITIZE_DEBUG "Sanitizers should be used in debug builds" YES)

option(SUPPLE_SANITIZE_RELEASE "Sanitizers should be used in release builds" NO)

option(SUPPLE_COMPILE_TESTS "Tests should be compiled" ${SUPPLE_MAIN_PROJECT})

option(SUPPLE_FULL_TESTS "Test C++20 and 23 as well" NO)

option(SUPPLE_OMIT_20
       "C++20 tests should be omitted - ignored if FULL_TESTS is false" NO)

option(SUPPLE_OMIT_23
       "C++23 tests should be omitted - ignored if FULL_TESTS is false" NO)

# Options specific to compiling with clang
option(SUPPLE_DO_CLANG_TIDY "Run clang-tidy during build" NO)
option(SUPPLE_DO_TIME_TRACE "Run time trace during build" NO)

supple_status_message("Build type: ${CMAKE_BUILD_TYPE}")

include(GNUInstallDirs)

add_library(supple_internal_core INTERFACE)
target_include_directories(
  supple_internal_core
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/supple/core)
target_compile_features(supple_internal_core INTERFACE cxx_std_17)

add_library(supple_internal_testing INTERFACE)
target_include_directories(
  supple_internal_testing
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/supple/testing)
target_link_libraries(supple_internal_testing INTERFACE supple::core)
target_compile_features(supple_internal_testing INTERFACE cxx_std_17)

add_library(supple_internal_full INTERFACE)
target_link_libraries(supple_internal_full INTERFACE supple::core supple::full)
target_compile_features(supple_internal_full INTERFACE cxx_std_17)

add_library(supple::core ALIAS supple_internal_core)
add_library(supple::testing ALIAS supple_internal_testing)
add_library(supple::full ALIAS supple_internal_full)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/supple-${PROJECT_VERSION})

write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion ARCH_INDEPENDENT)

install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/supple-${PROJECT_VERSION})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/supple
                  ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/supple
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(SUPPLE_COMPILE_TESTS)
  include(supple_testing)
  include(CTest)
  enable_testing()

  supple_test_handling()
endif()
