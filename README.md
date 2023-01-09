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

You can simply clone this repository into your `CMAKE_PREFIX_PATH`,
find the package `supple`.
The package currently defines three targets: `supple::core`, `supple::testing`, and `supple::full`.

* `supple::full` will always link all targets.
* `supple::core` contains the majority of the library's functionality.
* `supple::testing` contains the testing framework.

Alternatively, you can simply copy `cpp/inc/<module-name>` into your project, and include headers directly.

---

# How do I run the tests?

You may want to run the tests to ensure that this library works with your toolchain.
Please note that use with MSVC is entirely untested and unsupported.
This library is tested with gcc 9,10,11,12, clang 11,12,13,14,15, and Intel icc and icpx 2023.
If any tests fail with any other compiler which properly supports C++17,
please file a bug report.

To run the tests:

```
cmake -S cpp -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cmake --build build --target test
```

---

# What's with the name?

This project was formerly known as `supplementaries`,
but after I became aware of [another software project](https://github.com/MehVahdJukaar/Supplementaries)
of the same name, I figured I ought to change the name.

"Supple" was didn't require me to change the namespace,
and "supple" is how I'd been pronouncing the namespace
anyhow.
