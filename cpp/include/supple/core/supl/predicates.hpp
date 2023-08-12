/* {{{ doc */
/**
 * @file predicates.hpp
 *
 * @brief Unary predicate generators, and composition operations
 *
 * @details This header contains various unary predicate generators,
 * such as `equal_to`, `greater_than`, etc.
 * This header also contains functions to compose predicates
 * into new predicates.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_PREDICATES_HPP
#define SUPPLE_CORE_PREDICATES_HPP

#include <algorithm>
#include <tuple>
#include <type_traits>
#include <utility>

namespace supl {

/* {{{ doc */
/**
 * @brief Wrapper class for a unary predicate of class type,
 * which includes lambdas.
 */
/* }}} */
template <typename Pred>
class predicate : Pred
{
public:

  template <typename T,
            typename = std::enable_if_t<not std::is_same_v<T, predicate>>>
  // NOLINTNEXTLINE(*explicit*)
  constexpr predicate(T&& pred)
      : Pred {std::forward<T>(pred)}
  { }

  predicate() = delete;
  predicate(const predicate&) = default;
  constexpr predicate(predicate&&) noexcept = default;
  constexpr auto operator=(const predicate&) -> predicate& = default;
  constexpr auto operator=(predicate&&) noexcept -> predicate& = default;
  ~predicate() = default;

  using Pred::operator();
};

template <typename T>
predicate(T) -> predicate<T>;

constexpr inline auto true_pred {predicate {[](const auto&) {
  return true;
}}};

constexpr inline auto false_pred {predicate {[](const auto&) {
  return false;
}}};

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determines if its argument is
 * equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto equal_to(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg == parent_arg;
  }};
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument equals any of `args`
 *
 * @param args Values which returned predicate will compare against
 *
 * @return Unary predicate which determines if its argument
 * is equal to any of `args`
 */
/* }}} */
template <typename... Ts>
[[nodiscard]] constexpr auto equals_any_of(Ts&&... args) noexcept
{
  if constexpr ( sizeof...(args) == 0 ) {
    return false_pred;
  } else {
    return predicate {
      [parent_args_tup = std::tuple<std::decay_t<Ts>...> {std::forward<Ts>(
         args)...}](const auto& new_arg) constexpr noexcept -> bool {
        return std::apply(
          [&new_arg](auto&&... inner_args) {
            return ((new_arg == inner_args) || ...);
          },
          parent_args_tup);
      }};
  }
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is not equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determines if its argument is
 * not equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto not_equal_to(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg != parent_arg;
  }};
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is greater than `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determines if its argument is
 * greater than `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto greater_than(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg > parent_arg;
  }};
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is greater than or equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determines if its argument is
 * greater than or equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto greater_eq(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg >= parent_arg;
  }};
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is less than `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determines if its argument is
 * less than `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto less_than(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg < parent_arg;
  }};
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is less than or equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determines if its argument is
 * less than or equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto less_eq(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg <= parent_arg;
  }};
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is between two values
 *
 * @details "Between" is defined as being in the range
 * [min(bound1, bound2), max(bound1, bound2)].
 * `between(a, b)` is identical to `between(b, a)`.
 */
/* }}} */
template <typename T, typename U>
[[nodiscard]] constexpr auto between(T&& bound1, U&& bound2) noexcept
{
  return predicate {[lower_bound {std::min(bound1, bound2)},
                     upper_bound {std::max(bound1, bound2)}](
                      const auto& new_arg) constexpr noexcept -> bool {
    return lower_bound <= new_arg && new_arg <= upper_bound;
  }};
}

/* {{{ doc */
/**
 * @brief Create a unary predicate which determined if the input
 * is a multiple of `arg`
 *
 * @note Beware of using predicates returned from this function
 * and compositions thereof with floating-point values
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto multiple_of(T&& arg) noexcept
{
  return predicate {[parent_arg = std::forward<T>(arg)](
                      const auto& new_arg) constexpr noexcept -> bool {
    return new_arg % parent_arg == 0;
  }};
}

/* {{{ doc */
/**
 * @brief Create a unary prediate which determines if
 * the type of an argument satisfies a metafunction predicate
 */
/* }}} */
template <template <typename> typename PRED>
[[nodiscard]] constexpr auto type_pred() noexcept
{
  return predicate {[](auto&& arg) {
    return PRED<std::decay_t<decltype(arg)>>::value;
  }};
}

/* {{{ doc */
/**
 * @brief Create a unary predicate which determines if an argument
 * satisfies all of the provided predicates
 *
 * @details The returned predicate takes ownership of
 * the given predicates, do keep this in mind if
 * calling this function with nontrivial function objects.
 *
 * @param preds Pack of unary predicates
 *
 * @return Unary predicate equivalent to `(preds(arg) && ...)`,
 * where `arg` is the argument to the returned predicate.
 */
/* }}} */
template <typename... Preds>
[[nodiscard]] constexpr auto conjunction(Preds&&... preds) noexcept
{
  return predicate {
    [pred_tup {std::tuple<std::remove_reference_t<Preds>...> {
      std::forward<Preds>(preds)...}}](
      auto&& arg) constexpr noexcept -> bool {
      return std::apply(
        [&arg](auto&&... inner_preds) constexpr noexcept(
          noexcept((inner_preds(std::forward<decltype(arg)>(arg))&&...))) {
          return (inner_preds(std::forward<decltype(arg)>(arg)) && ...);
        },
        pred_tup);
    }};
}

/* {{{ doc */
/**
 * @brief Create a unary predicate which determines if an argument
 * satisfies any of the provided predicates
 *
 * @details The returned predicate takes ownership of
 * the given predicates, do keep this in mind if
 * calling this function with nontrivial function objects.
 *
 * @param preds Pack of unary predicates
 *
 * @return Unary predicate equivalent to `(preds(arg) || ...)`,
 * where `arg` is the argument to the returned predicate.
 */
/* }}} */
template <typename... Preds>
[[nodiscard]] constexpr auto disjunction(Preds&&... preds) noexcept
{
  return predicate {
    [pred_tup {std::tuple<std::remove_reference_t<Preds>...> {
      std::forward<Preds>(preds)...}}](
      auto&& arg) constexpr noexcept -> bool {
      return std::apply(
        [&arg](auto&&... inner_preds) constexpr noexcept(noexcept(
          (inner_preds(std::forward<decltype(arg)>(arg)) || ...))) {
          return (inner_preds(std::forward<decltype(arg)>(arg)) || ...);
        },
        pred_tup);
    }};
}

/* {{{ doc */
/**
 * @brief Logical negation of a predicate
 *
 * | pred | ret |
 * |:----:|:---:|
 * |  T   |  F  |
 * |  F   |  T  |
 */
/* }}} */
template <typename Pred>
[[nodiscard]] constexpr auto pred_not(Pred&& pred) noexcept
{
  return predicate {[inner_pred {std::forward<Pred>(pred)}](
                      auto&& arg) constexpr noexcept {
    return not inner_pred(std::forward<decltype(arg)>(arg));
  }};
}

/* {{{ doc */
/**
 * @brief `conjunction`, but accepts exactly 2 predicates
 * to reduce overhead
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  F  |
 * | F  | T  |  F  |
 * | F  | F  |  F  |
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto pred_and(Pred1&& pred1,
                                      Pred2&& pred2) noexcept
{
  return predicate {[inner_pred1 {pred1},
                     inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    return inner_pred1(std::forward<decltype(arg)>(arg))
        && inner_pred2(std::forward<decltype(arg)>(arg));
  }};
}

/* {{{ doc */
/**
 * @brief `disjunction`, but accepts exactly 2 predicates
 * to reduce overhead
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  T  |
 * | F  | T  |  T  |
 * | F  | F  |  F  |
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto pred_or(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return predicate {[inner_pred1 {pred1},
                     inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    return inner_pred1(std::forward<decltype(arg)>(arg))
        || inner_pred2(std::forward<decltype(arg)>(arg));
  }};
}

/* {{{ doc */
/**
 * @brief Logical xor of two predicates
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  F  |
 * | T  | F  |  T  |
 * | F  | T  |  T  |
 * | F  | F  |  F  |
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto pred_xor(Pred1&& pred1,
                                      Pred2&& pred2) noexcept
{
  return predicate {[inner_pred1 {pred1},
                     inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    return (inner_pred1(std::forward<decltype(arg)>(arg))
            + inner_pred2(std::forward<decltype(arg)>(arg)))
        == 1;
  }};
}

/* {{{ doc */
/**
 * @brief Logical implication of two predicates
 *
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  F  |
 * | F  | T  |  T  |
 * | F  | F  |  T  |
 *
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto pred_implies(Pred1&& pred1,
                                          Pred2&& pred2) noexcept
{
  return predicate {[inner_pred1 {pred1},
                     inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    if ( not inner_pred1(std::forward<decltype(arg)>(arg)) ) {
      return true;
    } else {
      return inner_pred2(std::forward<decltype(arg)>(arg));
    }
  }};
}

/* {{{ doc */
/**
 * @brief Logical biconditional of two predicates
 *
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  F  |
 * | F  | T  |  F  |
 * | F  | F  |  T  |
 *
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto pred_bicond(Pred1&& pred1,
                                         Pred2&& pred2) noexcept
{
  return pred_not(
    pred_xor(std::forward<Pred1>(pred1), std::forward<Pred2>(pred2)));
}

/* {{{ doc */
/**
 * @brief Logical negation of a predicate
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto operator!(const predicate<T>& pred) noexcept
{
  return pred_not(pred);
}

/* {{{ doc */
/**
 * @brief Logical conjunction (and) of two predicates
 */
/* }}} */
template <typename T, typename U>
[[nodiscard]] constexpr auto operator&&(const predicate<T>& pred1,
                                        const predicate<U>& pred2) noexcept
{
  return pred_and(pred1, pred2);
}

/* {{{ doc */
/**
 * @brief Logical disjunction (inclusive or) of two predicates
 */
/* }}} */
template <typename T, typename U>
[[nodiscard]] constexpr auto operator||(const predicate<T>& pred1,
                                        const predicate<U>& pred2) noexcept
{
  return pred_or(pred1, pred2);
}

/* {{{ doc */
/**
 * @brief Logical xor of two predicates
 */
/* }}} */
template <typename T, typename U>
[[nodiscard]] constexpr auto operator^(const predicate<T>& pred1,
                                       const predicate<U>& pred2) noexcept
{
  return pred_xor(pred1, pred2);
}

// There is no technical reason not to add operator equivalents
// of pred_implies or pred_bicond, but I cannot think of any
// operator which would make any sense
//
// If you wish to implement it yourself, it should be trivial
// if you copy-paste from the above binary operators,
// and define it in a `namespace supl` block, for reduced visibility

template <typename Arg, typename T>
[[nodiscard]] constexpr auto operator|(Arg&& arg,
                                       const predicate<T>& pred) noexcept
{
  return pred(arg);
}

}  // namespace supl

#endif
