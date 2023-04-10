# Supple

# What is this?

This is a small header-only library providing various handy utilities.
Some may not be handy, and may just be me having a neat idea.
Some loosely inspired by the STL, some just my ideas.
This library is written for C++17, but does work with later standards.

---

# Why is this?

I've made things I've felt might come in handy,
I've made things because I think they're neat,
I've made things just as a learning exercise.

---

# How do I use this?

You can simply add this repository as a submodule, and `add_subdirectory` in cmake,
or you may install it into your `CMAKE_PREFIX_PATH` and use `find_package`.
The package currently defines three targets: `supple::core`, `supple::testing`, and `supple::full`.

* `supple::full` will always link all targets.
* `supple::core` contains the majority of the library's functionality.
* `supple::testing` contains the testing framework.

Alternatively, you can simply copy `cpp/include/supple/<module-name>` into your project, and include headers directly,
listing the `<module-name>` directory as `-isystem`.
For example: if you copy `cpp/include/supple/core` to `my_cool_project/include/core`,
then you should pass the compiler option `-isystem include/core`.

---

# How do I run the tests?

You may want to run the tests to ensure that this library works with your toolchain.
Please note that use with MSVC is entirely untested and unsupported.
This library is tested with gcc 9, 10, 11, 12, clang 11, 12, 13, 14, 15, 16 and Intel icc 2021 and icpx 2023.
If any tests fail with any other compiler which properly supports C++17,
please file a bug report.

To run the tests:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cmake --build build --target test
```

Run `cmake -S . -B build -LH` for details on more options.
