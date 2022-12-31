cmake_minimum_required(VERSION 3.22.1)

if(TARGET supple::core)
  return()
endif()

add_library(supple::core INTERFACE IMPORTED)
target_include_directories(supple::core
                           INTERFACE ${CMAKE_CURRENT_LIST_DIR}/cpp/inc/core)
target_compile_features(supple::core INTERFACE cxx_std_17)

add_library(supple::full INTERFACE IMPORTED)
target_link_libraries(supple::full INTERFACE supple::core)
target_compile_features(supple::full INTERFACE cxx_std_17)

if(NOT DEFINED supple_FIND_QUIETLY)
  message("-- Found supple @ ${CMAKE_CURRENT_LIST_DIR}")
endif()
