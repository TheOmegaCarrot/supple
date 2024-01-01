/* {{{ doc */
/**
 * @file algorithm.hpp
 *
 * @brief Various algorithms
 *
 * @details This header contains various free functions intended
 * to sit alongside the standard `<algorithm>` header.
 * It has been the goal to adhere to similar API design and naming conventions.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_ALGORITHM_HPP
#define SUPPLE_CORE_ALGORITHM_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include "internal/algorithm/min_max.hpp"  //NOLINT

#include "metaprogramming.hpp"
#include "tuple_algo.hpp"

namespace supl {

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
 * @pre All parameters must have a `.size()` member function
 * which returns a `std::size_t`.
 * If this precondition is not satisfied, it is a compile-time error.
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
  return std::min(min_size(cont), min_size(conts...));
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
 * @pre All parameters must have a `.size()` member function
 * which returns a `std::size_t`.
 * If this precondition is not satisfied, it is a compile-time error.
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
  return std::max(max_size(cont), max_size(conts...));
}

template <typename Itr>
constexpr auto
contains(const Itr begin,
         const Itr end,
         sequential_apply_t<typename std::iterator_traits<Itr>::value_type,
                            std::add_const,
                            std::add_lvalue_reference>
           value) noexcept(noexcept(std::find(begin, end, value))) -> bool
{
  return std::find(begin, end, value) != end;
}

/* {{{ doc */
/**
 * @brief Conditionally transforms each element of a range.
 *
 * @pre `end` must be reachable by incrementing `begin`.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @pre All iterators in the half-open range [begin, end) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @tparam Itr Input iterator type. A forward iterator is sufficient.
 *
 * @tparam OutItr Output iterator type.
 * `OutItr = value; OutItr = another;` must insert `value` then `another`
 * into the output range to produce expected behavior.
 * Specializations of `std::back_insert_iterator`, `std::insert_iterator`,
 * and `std::front_insert_iterator` satisfy the preconditions, so long as
 * the underlying container contains an appropriate type.
 *
 * @tparam Predicate Type of unary predicate callable. Must accept a
 * single parameter of type `Itr::value_type`, and return a `bool`.
 *
 * @tparam TransformFunc Type of unary transforming callable. Must accept a
 * single parameter of type `Itr::value_type`, and return a value
 * implicitly convertible to `OutItr::container_type::value_type`.
 *
 * @param begin Beginning of input range.
 *
 * @param end End of input range.
 *
 * @param output_itr Output iterator to range used as destination of
 * transformed values.
 *
 * @param pred Unary predicate used to determine if an input value will be
 * transformed.
 *
 * @param func Unary function used to transform data.
 */
/* }}} */
template <typename Itr,
          typename OutItr,
          typename Predicate,
          typename TransformFunc>
constexpr void
transform_if(Itr begin,
             const Itr end,
             OutItr output_itr,
             Predicate&& pred,
             TransformFunc&& func) noexcept(noexcept(func(*begin)))
{
  for ( ; begin != end; ++begin ) {
    if ( pred(*begin) ) {
      *output_itr = func(*begin);
      ++output_itr;
    }
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each adjacent pair of elements.
 *
 * Example: Range is: {1, 2, 3}
 *
 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
 *
 * @pre `end` must be reachable by incrementing `begin`.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @pre All iterators in the half-open range [begin, end) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
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
  const Itr begin,
  const Itr end,
  BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  Itr leader {std::next(begin)};
  Itr follower {begin};

  for ( ; leader != end; ++leader, ++follower ) {
    func(*follower, *leader);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each adjacent pair of elements.
 * Iteration ceases when `n` iterations have occurred.
 * If `n` is greater than the size of the range,
 * behavior is undefined.
 *
 * Example: Range is: {1, 2, 3}
 *
 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
 *
 * @pre All iterators in the half-open range [begin, begin + n) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @tparam Itr Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin Iterator to the beginning of the range.
 *
 * @param n Maximum number of calls to make.
 *
 * @param func Binary function to apply to each adjacent pair.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
constexpr void for_each_adjacent_n(
  const Itr begin,
  const std::size_t n,
  BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  std::size_t count {0};
  Itr leader {std::next(begin)};
  Itr follower {begin};

  for ( ; count != n; ++count, ++leader, ++follower ) {
    func(*leader, *follower);
  }
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
 * @pre `end1` must be reachable by incrementing `begin1`.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @pre All iterators in the half-open range [begin1, end1) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @pre `end2` must be reachable by incrementing `begin2`.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @pre All iterators in the half-open range [begin2, end2) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
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
for_each_both(Itr1 begin1,
              const Itr1 end1,
              Itr2 begin2,
              const Itr2 end2,
              BinaryFunc&& func) noexcept(noexcept(func(*begin1, *begin2)))
{
  for ( ; (begin1 != end1 && begin2 != end2); ++begin1, ++begin2 ) {
    func(*begin1, *begin2);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each corresponding pair of elements.
 * Iteration ceases when `n` iterations have occurred.
 * If `n` is greater than the size of the range,
 * behavior is undefined.
 *
 * Example: Range 1 is: {1, 2, 3}, Range 2 is: {4, 5, 6, 7}
 *
 * Calls to `func` will be:
 *
 * * `func(1, 4)`
 * * `func(2, 5)`
 * * `func(3, 6)`
 *
 * @pre All iterators in the half-open range [begin1, begin1 + n) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
 *
 * @pre All iterators in the half-open range [begin2, begin2 + n) must be
 * dereferencable.
 * If this precondition is not satisfied, the result is undefined.
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
 * @param begin2 Iterator to the beginning of range 2
 *
 * @param n Maximum number of iterations.
 *
 * @param func Binary function to apply to each corresponding pair
 */
/* }}} */
template <typename Itr1, typename Itr2, typename BinaryFunc>
constexpr void for_each_both_n(
  Itr1 begin1,
  Itr2 begin2,
  const std::size_t n,
  BinaryFunc&& func) noexcept(noexcept(func(*begin1, *begin2)))
{
  for ( std::size_t count {0}; (count != n);
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
 * @pre All iterators in the half-open range [begins, begins + n) must be
 * dereferencable, for all iterators in the pack `begins`.
 * If this precondition is not satisfied, the result is undefined.
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
for_each_all_n(VarFunc&& func,
               const std::size_t n,
               Begins... begins) noexcept(noexcept(func(*begins...)))
{
  for ( std::size_t i {0}; i != n; ++i ) {
    func(*begins...);
    (++begins, ...);
  }
}

namespace impl {

  template <typename Tuple1, typename Tuple2, std::size_t... Idxs>
  constexpr auto
  tuple_elementwise_compare_any_impl(const Tuple1& tup1,
                                     const Tuple2& tup2,
                                     std::index_sequence<Idxs...>) noexcept
    -> bool
  {
    return ((std::get<Idxs>(tup1) == std::get<Idxs>(tup2)) || ...);
  }

  template <typename Tuple1, typename Tuple2>
  constexpr auto tuple_elementwise_compare_any(const Tuple1& tup1,
                                               const Tuple2& tup2) noexcept
    -> bool
  {
    static_assert(tl::size_v<Tuple1> == tl::size_v<Tuple2>);
    return tuple_elementwise_compare_any_impl(
      tup1, tup2, std::make_index_sequence<tl::size_v<Tuple1>> {});
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Applies `func` to elements of every passed range in parameter order.
 *
 * @details Iteration ceases when any `end` iterator is reached.
 * This operation may also be known as "zip".
 *
 * @pre Must be passed a pack of matching iterator pairs.
 * ex. `for_each_all(callable, begin1, end1, begin2, end2, begin3, end3)`
 * Failure to meet this precondition may be a compile-time error
 * if types mismatch,
 * or undefined behavior if types match,
 * but do not correspond to the same range.
 *
 * @pre `func` must be invocable with the value types of each iterator pair.
 *
 * @pre For example: the following would be a compile-time error
 * due to type mismatch,
 * as what would be called would be `function(int, long)`.
 *
 * ```
 * void function(int, char);
 *
 * int main() {
 * std::vector<int> int_vec {...};
 * std::vector<long> long_vec {...};
 * for_each_all(&function,
 * int_vec.begin(),
 * int_vec.end(),
 * long_vec.begin(),
 * long_vec.end()
 * )
 * };
 * ```
 *
 * @param func Callable to invoke with each set of range elements
 *
 * @param iterators Pack of iterators satisfying preconditions
 */
/* }}} */
template <typename VarFunc, typename... Iterators>
constexpr void for_each_all(VarFunc&& func,
                            Iterators... iterators) noexcept
{
  static_assert(sizeof...(Iterators) % 2 == 0,
                "Expected even number of iterators");

  static_assert(
    std::is_same_v<decltype(tuple::alternating_split(
                              std::tuple<Iterators&...> {iterators...})
                              .first),  // <- even indices in pack (begins)
                   decltype(tuple::alternating_split(
                              std::tuple<Iterators&...> {iterators...})
                              .second)>,  // <- odd indices in pack (ends)
    "Begin-end pairs must be the same type of iterator");

  for ( auto [begins, ends] {tuple::alternating_split(
          std::tuple<Iterators&...> {iterators...})};
        not impl::tuple_elementwise_compare_any(begins, ends);
        tuple::for_each(begins, [](auto& iterator) {
          ++iterator;
        }) ) {
    std::apply(
      [&func](auto&&... iterators_inner) mutable noexcept(
        noexcept(func(*iterators_inner...))) {
        return func(*iterators_inner...);
      },
      begins);
  }
}

/* {{{ doc */
/**
 * @brief constexpr re-implementation of `std::for_each`
 */
/* }}} */
template <typename Itr, typename Func>
constexpr void for_each(Itr begin,
                        const Itr end,
                        Func&& func) noexcept(noexcept(func(*begin)))
{
  for ( ; begin != end; ++begin ) {
    func(*begin);
  }
}

namespace impl {
  template <typename Func, typename Itr_Tuple, std::size_t... Idxs>
  constexpr void for_each_chain_impl(
    Func&& func,
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    Itr_Tuple& begins,
    Itr_Tuple& ends,
    std::index_sequence<Idxs...>) noexcept
  {
    (::supl::for_each(std::get<Idxs>(begins),
                      std::get<Idxs>(ends),
                      std::forward<Func>(func)),
     ...);
  }
}  // namespace impl

/* {{{ doc */
/**
 * @brief Chained `for_each`
 *
 * @details `for_each_chain(func, a.begin(), a.end(), b.begin(), b.end())`
 * is equivalent to `for_each(func, a.begin(), a.end()),
 * for_each(func, b.begin(), b.end()),`
 *
 * @pre Must be passed a pack of matching iterator pairs.
 * ex. `for_each_chain(callable, begin1, end1, begin2, end2, begin3, end3)`
 * Failure to meet this precondition may be a compile-time error
 * if types mismatch,
 * or a undefined behavior if types match,
 * but do not correspond to the same range.
 *
 * @pre `func` must be invocable with each of the value types of each iterator pair.
 *
 * @param func Unary callable invocable with the value type of each input range
 *
 * @param iterators Pack of iterators in begin-end pairs
 */
/* }}} */
template <typename Func, typename... Iterators>
constexpr void
for_each_chain(Func&& func, Iterators... iterators) noexcept(
  (noexcept(func(
     std::declval<typename std::iterator_traits<Iterators>::value_type>()))
   && ...))
{
  static_assert(sizeof...(Iterators) % 2 == 0,
                "Expected even number of iterators");

  static_assert(
    std::is_same_v<decltype(tuple::alternating_split(
                              std::tuple<Iterators&...> {iterators...})
                              .first),  // <- even indices in pack (begins)
                   decltype(tuple::alternating_split(
                              std::tuple<Iterators&...> {iterators...})
                              .second)>,  // <- odd indices in pack (ends)
    "Begin-end pairs must be the same type of iterator");

  auto [begins, ends] {
    tuple::alternating_split(std::tuple<Iterators&...>(iterators...))};
  impl::for_each_chain_impl(
    std::forward<Func>(func),
    begins,
    ends,
    std::make_index_sequence<sizeof...(Iterators) / 2> {});
}

/* {{{ doc */
/**
 * @brief Determine if all arguments satisfy a predicate
 *
 * @param pred Unary predicate all arguments must satisfy
 *
 * @return `true` if all arguments satisfy `pred`,
 * `false` otherwise. Returns `true` for zero arguments.
 */
/* }}} */
template <
  typename Pred,
  typename... Args,
  typename = std::enable_if<
    (std::is_same_v<bool,
                    decltype(std::declval<Pred>()(std::declval<Args>()))>
     && ...)
    || sizeof...(Args) == 0>>
[[nodiscard]] constexpr auto all_of(Pred&& pred, Args&&... args) noexcept(
  (noexcept(pred(std::forward<Args>(args))) && ...)) -> bool
{
  if constexpr ( sizeof...(Args) == 0 ) {
    return true;
  } else {
    return (pred(std::forward<Args>(args)) && ...);
  }
}

/* {{{ doc */
/**
 * @brief Determine if any arguments satisfy a predicate
 *
 * @param pred Unary predicate any arguments must satisfy
 *
 * @return `true` if any arguments satisfy `pred`,
 * `false` otherwise. Returns `false` for zero arguments.
 */
/* }}} */
template <
  typename Pred,
  typename... Args,
  typename = std::enable_if<
    (std::is_same_v<bool,
                    decltype(std::declval<Pred>()(std::declval<Args>()))>
     && ...)
    || sizeof...(Args) == 0>>
[[nodiscard]] constexpr auto any_of(Pred&& pred, Args&&... args) noexcept(
  (noexcept(pred(std::forward<Args>(args))) && ...)) -> bool
{
  if constexpr ( sizeof...(Args) == 0 ) {
    return false;
  } else {
    return (pred(std::forward<Args>(args)) || ...);
  }
}

/* {{{ doc */
/**
 * @brief Determine if no arguments satisfy a predicate
 *
 * @param pred Unary predicate no arguments must satisfy
 *
 * @return `true` if no arguments satisfy `pred`,
 * `false` otherwise. Returns `true` for zero arguments.
 */
/* }}} */
template <
  typename Pred,
  typename... Args,
  typename = std::enable_if<
    (std::is_same_v<bool,
                    decltype(std::declval<Pred>()(std::declval<Args>()))>
     && ...)
    || sizeof...(Args) == 0>>
[[nodiscard]] constexpr auto none_of(Pred&& pred, Args&&... args) noexcept(
  (noexcept(pred(std::forward<Args>(args))) && ...)) -> bool
{
  return not any_of(std::forward<Pred>(pred), std::forward<Args>(args)...);
}

/* {{{ doc */
/**
 * @brief Re-implementation of std::generate usable in a constexpr context.
 * Redundant if using >=C++20.
 *
 * @tparam Itr Iterator type
 *
 * @tparam Gen Parameterless function which returns `Itr::value_type`
 *
 * @param begin Beginning of range to be generated over.
 *
 * @param end End of range to be generated over.
 *
 * @param gen Parameterless function which returns `Itr::value_type`
 */
/* }}} */
template <typename Itr, typename Gen>
constexpr void generate(Itr begin, const Itr end, Gen&& gen) noexcept(
  noexcept(gen()) && noexcept(*begin))
{
  while ( begin != end ) {
    *begin = gen();
    ++begin;
  }
}

template <typename Itr, typename OutItr, typename TransformFunc>
constexpr void
transform(Itr begin,
          const Itr end,
          OutItr output_itr,
          TransformFunc&& func) noexcept(noexcept(func(*begin)))
{
  for ( ; begin != end; ++begin ) {
    *output_itr = func(*begin);
    ++output_itr;
  }
}

template <typename InItr, typename OutItr>
constexpr auto copy(InItr begin, const InItr end, OutItr out) noexcept(
  std::is_nothrow_copy_constructible_v<
    typename std::iterator_traits<InItr>::value_type>) -> OutItr
{
  for ( ; begin != end; ++out, ++begin ) {
    *out = *begin;
  }

  return out;
}

template <typename InItr, typename OutItr>
constexpr auto
copy_n(InItr begin, const std::size_t n, OutItr out) noexcept(
  std::is_nothrow_copy_constructible_v<
    typename std::iterator_traits<InItr>::value_type>) -> OutItr
{
  for ( std::size_t i {0}; i != n; ++i, ++out, ++begin ) {
    *out = *begin;
  }

  return out;
}

}  // namespace supl

#endif
