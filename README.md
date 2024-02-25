# Supple

# What is this?

This is a small header-only library providing various handy utilities,
with no dependencies beyond a compliant C++ standard library.
Some utilities inspired by the STL, some are just my ideas.
This library is written for C++17, but does work with later standards.

---

# What Kinds of Things are Offered?

This library is largely an assortment of small utilities, such as:
- More STL-like algorithms
- Support for directly formatting more types using `<iostream>`, without polluting the global namespace
- Sequence container operations on tuples
- Template metaprogramming utilities
    - Including type list operations
- Composable unary predicates
- CRTP helper classes
- A macro-free simple testing framework

---

# How do I use this?

You can simply add this repository as a submodule, and `add_subdirectory` in cmake,
or you may install it into your `CMAKE_PREFIX_PATH` and use `find_package`.
The package currently defines three targets: `supple::core`, `supple::testing`, and `supple::full`.

* `supple::full` will always link all targets except `supple::testing`.
* `supple::core` contains the majority of the library's functionality.
* `supple::testing` contains the testing framework.

Alternatively, you can simply copy `cpp/include/supple/<module-name>` into your project, and include headers directly,
listing the `<module-name>` directory as `-isystem`.
For example: if you copy `cpp/include/supple/core` to `my_project/include/supple/core`,
then you should pass the compiler option `-isystem include/supple/core`.

---

# How do I run the tests?

You may want to run the tests to ensure that this library works with your toolchain.
This library is tested with gcc 9, 10, 11, 12, and 13, clang 11, 12, 13, 14, 15, 16, and 17, Intel icc 2021, Intel icpx 2023 and 2024, and MSVC 19.35.
If any tests fail with any compiler which properly supports C++17,
please open an issue.

To run the tests:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cmake --build build --target test
```

# What if I want to install this without running the tests?

You can set the option `-DSUPPLE_COMPILE_TESTS=NO` when configuring. For example:

```
cmake -S . -B build -DSUPPLE_COMPILE_TESTS=NO -DCMAKE_INSTALL_PREFIX=/wherever/you/want
```

Run `cmake -S . -B build -LH` for details on more options.
