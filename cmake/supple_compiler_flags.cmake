add_library(supple_compiler_flags INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # set warnings for GCC
  target_compile_options(
    supple_compiler_flags
    INTERFACE -Wall
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
  target_compile_options(
    supple_compiler_flags
    INTERFACE -Weverything
              -Wno-unknown-warning-option
              -Wno-ctad-maybe-unsupported
              -Wno-missing-prototypes
              -Wno-padded
              -Wno-c++98-compat
              -Wno-c++98-compat-pedantic
              -Wno-unreachable-code
              -Wno-weak-vtables
              -Wno-unsafe-buffer-usage)

  # Use clang-tidy if not an automated build by cmake-all.sh
  if(SUPPLE_DO_CLANG_TIDY)
    supple_status_message("Using clang-tidy")
    set(CMAKE_CXX_CLANG_TIDY
        "clang-tidy;--config-file=${CMAKE_CURRENT_SOURCE_DIR}/cpp/.clang-tidy")
  endif()

  if(SUPPLE_DO_TIME_TRACE)
    supple_status_message("Running time trace")
    target_compile_options(supple_compiler_flags INTERFACE -ftime-trace)
  endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  target_compile_options(
    supple_compiler_flags
    INTERFACE -diag-enable=all -diag-disable=10441 -diag-disable=13000
              -diag-disable=11074 -diag-disable=11076)

  target_link_options(
    supple_compiler_flags
    INTERFACE
    -diag-enable=all
    -diag-disable=10441
    -diag-disable=13000
    -diag-disable=11074
    -diag-disable=11076)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
  target_compile_options(
    supple_compiler_flags INTERFACE -diag-enable=all
                                    -Rno-debug-disables-optimization)

  target_link_options(supple_compiler_flags INTERFACE -diag-enable=all
                      -Rno-debug-disables-optimization)

elseif(MSVC)
  target_compile_options(
    supple_compiler_flags INTERFACE /permissive-)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                           "Clang")

  # GCC and Clang are the only compilers I have which support sanitizers
  if(SUPPLE_SANITIZE_DEBUG AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    supple_status_message("Using sanitizers in debug")
    set(SANITIZERS address,leak,undefined)

    target_compile_options(supple_compiler_flags
                           INTERFACE -fsanitize=${SANITIZERS})

    target_link_options(supple_compiler_flags INTERFACE
                        -fsanitize=${SANITIZERS})

  endif()

  if(SUPPLE_SANITIZE_RELEASE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    supple_status_message("Using sanitizers in release")
    set(SANITIZERS address,leak,undefined)

    target_compile_options(supple_compiler_flags
                           INTERFACE -fsanitize=${SANITIZERS})

    target_link_options(supple_compiler_flags INTERFACE
                        -fsanitize=${SANITIZERS})

  endif()

endif()
