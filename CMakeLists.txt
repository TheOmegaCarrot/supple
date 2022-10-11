cmake_minimum_required(VERSION 3.13.4)

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
    add_compile_options(-O${OPT})
endmacro()

if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    message("-- Using GCC")

    add_compile_options(-Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wredundant-decls -Wconversion -Wnon-virtual-dtor -Wunused -Wdouble-promotion -Wformat=2 -Wnull-dereference -Wimplicit-fallthrough -Wuninitialized -Wstrict-overflow=4 -Wold-style-cast -Wfloat-equal -Wundef -Wshadow -Wfree-nonheap-object -Wsign-conversion -Wno-multichar -Woverloaded-virtual -Wctor-dtor-privacy -Wsign-promo -Wmisleading-indentation -Wzero-as-null-pointer-constant -Wsuggest-override -Weffc++ -Wno-return-type -Wno-error=effc++ -Werror)
    sanitize()
    optimize(0)

elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    message("-- Using Clang")

    add_compile_options(-Weverything -Wno-ctad-maybe-unsupported -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic)
    sanitize()
    optimize(0)

elseif ( MSVC )
    message("-- Using MSVC")
    # Untested
endif()

include_directories(inc)

file(GLOB_RECURSE source_files src/*.cpp)
list(REMOVE_ITEM source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(${PROJECT_NAME} "src/main.cpp" ${source_files})


file(GLOB_RECURSE test_files "tst/src/*.cpp")
add_executable(run_tests ${source_files} ${test_files})
target_include_directories(run_tests PRIVATE tst/inc)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${standard})
set_property(TARGET run_tests PROPERTY CXX_STANDARD ${standard})
