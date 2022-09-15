#ifndef EHANC_UTILS_ALGORITHM_HPP
#define EHANC_UTILS_ALGORITHM_HPP

#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

namespace ehanc {

/* {{{ doc */
/**
 * @brief Base case of recursive overload.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @param cont Container whose size to query.
 *
 * @return Size of container `cont`.
 */
/* }}} */
template <typename Container>
constexpr auto min_size(const Container& cont) noexcept -> std::size_t
{
  return cont.size();
}

/* {{{ doc */
/**
 * @brief Returns size of smallest container provided.
 * Calls size method internally.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @tparam Containers Sequence of container types which all provide
 * a `.size()` member function. Types may be mixed, so long as all
 * `.size()` member functions return comparable types which can be
 * passed to `std::min`.
 *
 * @param cont First container.
 *
 * @param conts Pack of remaining containers.
 *
 * @return Size of smallest container.
 */
/* }}} */
template <typename Container, typename... Containers>
constexpr auto min_size(const Container& cont,
                        const Containers&... conts) noexcept -> std::size_t
{
  return std::min(::ehanc::min_size(cont), ::ehanc::min_size(conts...));
}

/* {{{ doc */
/**
 * @brief Base case of recursive overload.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @param cont Container whose size to query.
 *
 * @return Size of container `cont`.
 */
/* }}} */
template <typename Container>
constexpr auto max_size(const Container& cont) noexcept -> std::size_t
{
  return cont.size();
}

/* {{{ doc */
/**
 * @brief Returns size of largest container provided.
 * Calls size method internally.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @tparam Containers Sequence of container types which all provide
 * a `.size()` member function. Types may be mixed, so long as all
 * `.size()` member functions return comparable types which can be
 * passed to `std::max`.
 *
 * @param cont First container.
 *
 * @param conts Pack of remaining containers.
 *
 * @return Size of smallest container.
 */
/* }}} */
template <typename Container, typename... Containers>
constexpr auto max_size(const Container& cont,
                        const Containers&... conts) noexcept -> std::size_t
{
  return std::max(::ehanc::max_size(cont), ::ehanc::max_size(conts...));
}

/* {{{ doc */
/**
 * @brief Applies `func` to each adjacent pair of elements.
 *
 * Example: Range is: {1, 2, 3}
 *
 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
 *
 * @tparam Itr Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin Iterator to the beginning of the range.
 *
 * @param end Iterator to the end of the range.
 *
 * @param func Binary function to apply to each adjacent pair.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
constexpr void for_each_adjacent(
    const Itr begin, const Itr end,
    BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  Itr leader {std::next(begin)};
  Itr follower {begin};

  for ( ; leader != end; ++leader, ++follower ) {
    func(*leader, *follower);
  }
}

/* {{{ doc */
/**
 * @brief Forwarding function alias for for_each_adjacent following rename.
 *
 * @deprecated Renamed to `for_each_adjacent`.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
[[deprecated("Use for_each_adjacent")]] constexpr void
for_each_pair(Itr&& begin, Itr&& end,
              BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  ::ehanc::for_each_adjacent(std::forward<Itr>(begin),
                             std::forward<Itr>(end),
                             std::forward<BinaryFunc>(func));
}

/* {{{ doc */
/**
 * @brief Applies `func` to each adjacent pair of elements.
 * Iteration ceases when the range runs out of adjacent pairs,
 * or when `n` iterations have occurred; whichever comes first.
 *
 * Example: Range is: {1, 2, 3}
 *
 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
 *
 * @tparam Itr Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin Iterator to the beginning of the range.
 *
 * @param end Iterator to the end of the range.
 *
 * @param n Maximum number of calls to make.
 *
 * @param func Binary function to apply to each adjacent pair.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
constexpr void for_each_adjacent_n(
    const Itr begin, const Itr end, const std::size_t n,
    BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  std::size_t count {0};
  Itr leader {std::next(begin)};
  Itr follower {begin};

  for ( ; count != n && leader != end; ++count, ++leader, ++follower ) {
    func(*leader, *follower);
  }
}

/* {{{ doc */
/**
 * @brief Forwarding function alias for for_each_adjacent following rename.
 *
 * @deprecated Renamed to `for_each_adjacent_n`.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
[[deprecated("Use for_each_adjacent_n")]] constexpr void
for_each_pair_n(Itr&& begin, Itr&& end, const std::size_t n,
                BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  ::ehanc::for_each_adjacent_n(std::forward<Itr>(begin),
                               std::forward<Itr>(end), n,
                               std::forward<BinaryFunc>(func));
}

/* {{{ doc */
/**
 * @brief Applies `func` to each corresponding pair of elements.
 * Iteration ceases when either range runs out of members.
 *
 * Example: Range 1 is: {1, 2, 3}, Range 2 is: {4, 5, 6, 7}
 *
 * Calls to `func` will be:
 *
 * * `func(1, 4)`
 * * `func(2, 5)`
 * * `func(3, 6)`
 *
 * @tparam Itr1 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam Itr2 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin1 Iterator to the beginning of range 1
 *
 * @param end1 Iterator to the end of range 1
 *
 * @param begin2 Iterator to the beginning of range 2
 *
 * @param end2 Iterator to the end of range 2
 *
 * @param func Binary function to apply to each corresponding pair
 */
/* }}} */
template <typename Itr1, typename Itr2, typename BinaryFunc>
constexpr void
for_each_both(Itr1 begin1, const Itr1 end1, Itr2 begin2, const Itr2 end2,
              BinaryFunc&& func) noexcept(noexcept(func(*begin1, *begin2)))
{
  for ( ; (begin1 != end1 && begin2 != end2); ++begin1, ++begin2 ) {
    func(*begin1, *begin2);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each corresponding pair of elements.
 * Iteration ceases when either range runs out of members,
 * or when `n` iterations have occurred; whichever comes first.
 *
 * Example: Range 1 is: {1, 2, 3}, Range 2 is: {4, 5, 6, 7}
 *
 * Calls to `func` will be:
 *
 * * `func(1, 4)`
 * * `func(2, 5)`
 * * `func(3, 6)`
 *
 * @tparam Itr1 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam Itr2 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin1 Iterator to the beginning of range 1
 *
 * @param end1 Iterator to the end of range 1
 *
 * @param begin2 Iterator to the beginning of range 2
 *
 * @param end2 Iterator to the end of range 2
 *
 * @param n Maximum number of iterations.
 *
 * @param func Binary function to apply to each corresponding pair
 */
/* }}} */
template <typename Itr1, typename Itr2, typename BinaryFunc>
constexpr void for_each_both_n(
    Itr1 begin1, const Itr1 end1, Itr2 begin2, const Itr2 end2,
    const std::size_t n,
    BinaryFunc&& func) noexcept(noexcept(func(*begin1, *begin2)))
{
  for ( std::size_t count {0};
        (count != n && begin1 != end1 && begin2 != end2);
        ++count, ++begin1, ++begin2 ) {
    func(*begin1, *begin2);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to members of
 * all containers in parameter order.
 * Iteration ceases when after `n` iterations.
 * If `n` is greater than the size of the smallest container,
 * behavior is undefined.
 *
 * Example: Range1: {1, 2, 3}, Range2: {'a', 'b', 'c'}
 *
 * `func` must have accept `int, char` as parameters.
 *
 * @tparam VarFunc A function which acccepts the types of all the
 * contaners in parameter order.
 *
 * @tparam Begins Sequence of iterators satisfying only a minimal
 * inteface. Simply being incrementable is sufficient.
 *
 * @param func A function which accepts the types of all the containers
 * in parameter order.
 *
 * @param n Maximum number of iterations. Must not be greater
 * than the size of the smallest container.
 *
 * @param begins Iterators to containers to be iterated over.
 * Smallest distance from any to its corresponding end must not be less
 * than `n`, or behavior will be undefined.
 */
/* }}} */
template <typename VarFunc, typename... Begins>
constexpr void
for_each_all_n(VarFunc&& func, const std::size_t n,
               Begins... begins) noexcept(noexcept(func(*begins...)))
{
  for ( std::size_t i {0}; i != n; ++i ) {
    func(*begins...);
    (++begins, ...);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to members of all container in
 * parameter order. Iteration ceases upon reaching the end of
 * any container.
 *
 * @tparam VarFunc A function which acccepts the types of all the
 * contaners in parameter order.
 *
 * @tparam Containers Sequence of containers which provide
 * iterators capable of incrementing and dereferencing.
 *
 * @param func A function which accepts the types of all the containers
 * in parameter order.
 *
 * @param containers Parameter pack of containers to be
 * iterated over.
 */
/* }}} */
template <typename VarFunc, typename... Containers>
constexpr void
for_each_all(VarFunc&& func, Containers&... containers) noexcept(
    noexcept(::ehanc::for_each_all_n(std::forward<VarFunc>(func),
                                     ::ehanc::min_size(containers...),
                                     std::begin(containers)...)))
{
  ::ehanc::for_each_all_n(std::forward<VarFunc>(func),
                          ::ehanc::min_size(containers...),
                          std::begin(containers)...);
}

/* {{{ doc */
/**
 * @brief Applies `func` to members of all container in
 * parameter order. Iteration ceases upon reaching the end of
 * any container. Ensures the containers are not modified.
 *
 * @tparam VarFunc A function which acccepts the types of all the
 * contaners in parameter order.
 *
 * @tparam Containers Sequence of containers which provide
 * iterators capable of incrementing and dereferencing.
 *
 * @param func A function which accepts the types of all the containers
 * in parameter order.
 *
 * @param containers Parameter pack of const containers to be
 * iterated over.
 */
/* }}} */
template <typename VarFunc, typename... Containers>
constexpr void
for_each_all_c(VarFunc&& func, const Containers&... containers) noexcept(
    noexcept(for_each_all_n(std::forward<VarFunc>(func),
                            ::ehanc::min_size(containers...),
                            std::cbegin(containers)...)))
{
  ::ehanc::for_each_all_n(std::forward<VarFunc>(func),
                          ::ehanc::min_size(containers...),
                          std::cbegin(containers)...);
}

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Not
 * intended to be called outside ::ehanc::for_each_in_tuple.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Function type.
 *
 * @tparam Inds Sequence of integers for template wizardry.
 *
 * @param tup Tuple to apply function to a member of.
 *
 * @param func Function to apply to a member of tuple.
 *
 * @param magic std::index_sequence just there for deducing Inds for
 * template wizardry.
 */
/* }}} */
template <typename Tuple, typename Func, std::size_t... Inds>
constexpr void
for_each_in_tuple_impl(const Tuple& tup, Func&& func,
                       [[maybe_unused]] std::index_sequence<Inds...> magic)
{
  (func(std::get<Inds>(tup)), ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Would not
 * have been possible without what I learned from this CppCon talk:
 * https://www.youtube.com/watch?v=15etE6WcvBY
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Visitor function type.
 *
 * @param tup Tuple to apply a visitor function to every element of.
 *
 * @param func Visitor function.
 */
/* }}} */
template <typename Tuple, typename Func>
constexpr void for_each_in_tuple(const Tuple& tup, Func&& func) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  ::ehanc::impl::for_each_in_tuple_impl(tup, func, seq);
}

inline namespace bkprt {

template <typename Itr, typename Gen>
constexpr void
generate(Itr begin, const Itr end,
         Gen&& gen) noexcept(noexcept(gen()) && noexcept(*begin))
{
  while ( begin != end ) {
    *begin = gen();
    ++begin;
  }
}

} // namespace bkprt

} // namespace ehanc

#endif
