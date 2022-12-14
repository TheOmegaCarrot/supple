# What is this document?

This is a list of ideas that may or may not be worth implementing.

---

## Tuple Algorithms

### Emplace_{Back,Front}:

Construct a new element from constructor arguments.

Could be used to save a move.

Perhaps also just a `tuple_emplace` that works like `tuple_insert`?

```cpp
template<typename Tuple, typename To_Be_Constructed, typename... Args>
auto tuple_emplace_{back,front}(const Tuple& tup, Args&&... args);
```
