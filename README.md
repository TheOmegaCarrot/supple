# Supplementaries

# What is this?

This is a small header-only library providing various handy utilities.
Some may not be handy, and may just be me having a neat idea.
Some loosely inspired by the STL, some just my ideas.
This library is written with only C++17 in mind.

---

# Why is this?

I've made things I've felt might come in handy,
I've made things because I think they're neat,
I've made things just as a learning exercise.

---

# How do I use this?

You can simply clone this repository into your `CMAKE_PREFIX_PATH`,
find the package `supplementaries`.
The package currently defines two targets: `supplementaries::core`, and `supplementaries::all`.
Currently, there is no difference, however, more modules may be added in the future.
If and when more modules are added, `supplementaries::all` will include all modules,
while `supplementaries::core` is only one module.

Alternatively, you can simply copy `cpp/inc/supl` into your project, and include headers directly.
