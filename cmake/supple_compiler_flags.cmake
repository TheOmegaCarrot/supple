add_library(supple_compiler_flags INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # set warnings for GCC
  target_compile_options(supple_compiler_flags
                         INTERFACE -fdiagnostics-color=always)

  target_compile_options(
    supple_compiler_flags
    INTERFACE -Wall
              -Wextra
              -pedantic
              -Wno-unknown-warning-option
              -Waddress
              -Waddress-of-packed-member
              -Waggressive-loop-optimizations
              -Waligned-new=all
              -Wall
              -Walloca
              -Walloc-zero
              -Warith-conversion
              -Warray-bounds=2
              -Warray-compare
              -Warray-parameter=2
              -Wattribute-alias=2
              -Wattributes
              -Wattribute-warning
              -Wbidi-chars=any
              -Wbool-compare
              -Wbool-operation
              -Wbuiltin-declaration-mismatch
              -Wbuiltin-macro-redefined
              -Wc++20-compat
              -Wc++20-extensions
              -Wc++23-extensions
              -Wcannot-profile
              -Wcast-align=strict
              -Wcast-function-type
              -Wcast-qual
              -Wcatch-value=3
              -Wchanges-meaning
              -Wchar-subscripts
              -Wclass-conversion
              -Wclass-memaccess
              -Wclobbered
              -Wcomma-subscript
              -Wcomplain-wrong-lang
              -Wconditionally-supported
              -Wconversion
              -Wconversion-null
              -Wcoverage-invalid-line-number
              -Wcoverage-mismatch
              -Wcpp
              -Wctor-dtor-privacy
              -Wdangling-else
              -Wdangling-pointer=2
              -Wdangling-reference
              -Wdate-time
              -Wdelete-incomplete
              -Wdelete-non-virtual-dtor
              -Wdeprecated
              -Wdeprecated-copy
              -Wdeprecated-copy-dtor
              -Wdeprecated-declarations
              -Wdeprecated-enum-enum-conversion
              -Wdeprecated-enum-float-conversion
              -Wdisabled-optimization
              -Wdiv-by-zero
              -Wdouble-promotion
              -Wduplicated-branches
              -Wduplicated-cond
              -Wempty-body
              -Wendif-labels
              -Wenum-compare
              -Wenum-conversion
              -Wexceptions
              -Wexpansion-to-defined
              -Wextra
              -Wextra-semi
              -Wfloat-conversion
              -Wfloat-equal
              -Wformat
              -Wformat=2
              -Wformat-contains-nul
              -Wformat-diag
              -Wformat-extra-args
              -Wformat-nonliteral
              -Wformat-overflow=2
              -Wformat-security
              -Wformat-signedness
              -Wformat-truncation=2
              -Wformat-y2k
              -Wformat-zero-length
              -Wframe-address
              -Wfree-nonheap-object
              -Whsa
              -Wif-not-aligned
              -Wignored-attributes
              -Wignored-qualifiers
              -Wimplicit-fallthrough=5
              -Winaccessible-base
              -Winfinite-recursion
              -Winherited-variadic-ctor
              -Winit-list-lifetime
              -Winit-self
              -Winline
              -Winterference-size
              -Wint-in-bool-context
              -Wint-to-pointer-cast
              -Winvalid-constexpr
              -Winvalid-imported-macros
              -Winvalid-memory-model
              -Winvalid-offsetof
              -Winvalid-pch
              -Winvalid-utf8
              -Wliteral-suffix
              -Wlogical-not-parentheses
              -Wlogical-op
              -Wlto-type-mismatch
              -Wmain
              -Wmaybe-uninitialized
              -Wmemset-elt-size
              -Wmemset-transposed-args
              -Wmisleading-indentation
              -Wmismatched-dealloc
              -Wmismatched-new-delete
              -Wmismatched-tags
              -Wmissing-attributes
              -Wmissing-field-initializers
              -Wmissing-format-attribute
              -Wmissing-include-dirs
              -Wmissing-noreturn
              -Wmissing-profile
              -Wmissing-template-keyword
              -Wmultichar
              -Wmultistatement-macros
              -Wnarrowing
              # -Wnoexcept # seems to give a lot of false positives?
              -Wnoexcept-type
              -Wnonnull
              -Wnonnull-compare
              -Wnon-template-friend
              -Wnon-virtual-dtor
              -Wnormalized=nfc
              -Wnull-dereference
              -Wodr
              -Wold-style-cast
              -Wopenacc-parallelism
              -Wopenmp-simd
              -Woverflow
              -Woverlength-strings
              -Woverloaded-virtual=2
              -Wpacked
              -Wpacked-bitfield-compat
              -Wpacked-not-aligned
              -Wparentheses
              -Wpedantic
              -Wpessimizing-move
              -Wplacement-new=2
              -Wpmf-conversions
              -Wpointer-arith
              -Wpointer-compare
              -Wpragmas
              -Wprio-ctor-dtor
              -Wpsabi
              -Wrange-loop-construct
              -Wredundant-decls
              -Wredundant-move
              -Wredundant-tags
              -Wregister
              -Wreorder
              -Wrestrict
              -Wreturn-local-addr
              -Wreturn-type
              -Wscalar-storage-order
              -Wself-move
              -Wsequence-point
              -Wshadow
              -Wshadow-compatible-local
              -Wshadow=compatible-local
              -Wshadow=global
              -Wshadow-local
              -Wshadow=local
              -Wshift-count-negative
              -Wshift-count-overflow
              -Wshift-negative-value
              -Wshift-overflow=2
              -Wsign-compare
              -Wsign-conversion
              -Wsign-promo
              -Wsized-deallocation
              -Wsizeof-array-argument
              -Wsizeof-array-div
              -Wsizeof-pointer-div
              -Wsizeof-pointer-memaccess
              -Wstack-protector
              -Wstrict-aliasing=3
              -Wstrict-null-sentinel
              -Wstrict-overflow=5
              -Wstring-compare
              -Wstringop-overflow=4
              -Wstringop-overread
              -Wstringop-truncation
              -Wsubobject-linkage
              -Wsuggest-final-methods
              -Wsuggest-final-types
              -Wsuggest-override
              -Wswitch
              -Wswitch-bool
              -Wswitch-default
              -Wswitch-enum
              -Wswitch-outside-range
              -Wswitch-unreachable
              -Wsync-nand
              -Wsynth
              -Wtautological-compare
              -Wterminate
              -Wtrampolines
              -Wtrigraphs
              -Wtrivial-auto-var-init
              -Wtsan
              -Wtype-limits
              -Wundef
              -Wunicode
              -Wuninitialized
              -Wunknown-pragmas
              -Wunreachable-code
              -Wunsafe-loop-optimizations
              -Wunused
              -Wunused-but-set-parameter
              -Wunused-but-set-variable
              -Wunused-const-variable=2
              -Wunused-function
              -Wunused-label
              -Wunused-local-typedefs
              -Wunused-macros
              -Wunused-parameter
              -Wunused-result
              -Wunused-value
              -Wunused-variable
              -Wuse-after-free=3
              -Wuseless-cast
              -Wvarargs
              -Wvariadic-macros
              -Wvector-operation-performance
              -Wvexing-parse
              -Wvirtual-inheritance
              -Wvirtual-move-assign
              -Wvla
              -Wvolatile
              -Wvolatile-register-var
              -Wwrite-strings
              -Wxor-used-as-pow
              -Wzero-as-null-pointer-constant
              -Wzero-length-bounds)

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
  target_compile_options(supple_compiler_flags INTERFACE /permissive-)
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

# Use clang-tidy if not an automated build by cmake-all.sh
if(SUPPLE_DO_CLANG_TIDY)
  supple_status_message("Using clang-tidy")
  set(CMAKE_CXX_CLANG_TIDY
      "clang-tidy;--config-file=${CMAKE_CURRENT_SOURCE_DIR}/cpp/.clang-tidy")
endif()
