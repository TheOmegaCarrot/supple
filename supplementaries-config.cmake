cmake_minimum_required(VERSION 3.22.1)

if(TARGET supplementaries::core)
  return()
endif()

add_library(supplementaries::core INTERFACE IMPORTED)
target_include_directories(supplementaries::core
                           INTERFACE ${CMAKE_CURRENT_LIST_DIR}/cpp/inc/core)
target_compile_features(supplementaries::core INTERFACE cxx_std_17)

add_library(supplementaries::full INTERFACE IMPORTED)
target_link_library(supplementaries::full PUBLIC supplementaries::core)
target_compile_features(supplementaries::full INTERFACE cxx_std_17)

if(NOT DEFINED supplementaries_FIND_QUIETLY)
  message("-- Found supplementaries @ ${CMAKE_CURRENT_LIST_DIR}")
endif()
