cmake_minimum_required(VERSION 3.22.1)
project(supplementaries LANGUAGES CXX)
set(standard 17)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE cpp/inc)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${standard})

