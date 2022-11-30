cmake_minimum_required(VERSION 3.22.1)
project(supplementaries LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE cpp/inc)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
