cmake_minimum_required(VERSION 3.22.1)

if(TARGET supplementaries::supplementaries)
  return()
endif()

add_library(supplementaries::supplementaries INTERFACE IMPORTED)
target_include_directories(supplementaries::supplementaries INTERFACE ${CMAKE_CURRENT_LIST_DIR}/cpp/inc)
target_compile_features(supplementaries::supplementaries INTERFACE cxx_std_17)
