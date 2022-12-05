# What is this document?

This is a list of ideas that may or may not be worth implementing.

---

## Tuple Algorithms

### Swap:

Simply swap two elements of a tuple.

```cpp
template<typename Tuple, std::size_t Idx1, std::size_t Idx2>
auto tuple_swap(const Tuple& tup, supl::index_pair<Idx1, Idx2>);

tuple_swap(std::tuple{42, 3.14, false}, supl::index_pair<0,2>) == std::tuple{false, 3.14, 42}
```

### Emplace_{Back,Front}:

Construct a new element from constructor arguments.

Could be used to save a move.

Perhaps also just a `tuple_emplace` that works like `tuple_insert`?

```cpp
template<typename Tuple, typename To_Be_Constructed, typename... Args>
auto tuple_emplace_{back,front}(const Tuple& tup, Args&&... args);
```

### Rotate_{Left,Right}:

Yes, this *could* be done with `tuple_reorder`, but that would be much less clear, and would be more clunky and error prone.

```cpp
template<typename Tuple>
auto tuple_rotate_{left,right}(const Tuple& tup);

tuple_rotate_left(std::tuple{42, 3.14, false}) == std::tuple{3.14, false, 42}
```
