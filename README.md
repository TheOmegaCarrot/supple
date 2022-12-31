# Supple

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
find the package `supple`.
The package currently defines two targets: `supple::core`, and `supple::full`.
Currently, there is no difference, however, more modules may be added in the future.
If and when more modules are added, `supple::full` will include all modules,
while `supple::core` is only one module.

Alternatively, you can simply copy `cpp/inc/supl` into your project, and include headers directly.

---

# What's with the name?

This project was formerly known as `supplementaries`,
but after I became aware of [another software project](https://github.com/MehVahdJukaar/Supplementaries)
of the same name, I figured I ought to change the name.

"Supple" was didn't require me to change the namespace,
and "supple" is how I'd been pronouncing the namespace
anyhow.
