#ifndef SUPPLE_CORE_PREDICATES_HPP
#define SUPPLE_CORE_PREDICATES_HPP

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <supl/metaprogramming.hpp>
#include <supl/tuple_algo.hpp>

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

  template <
    typename T,
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
  return predicate {
    [parent_arg = std::forward<T>(arg)](const auto& new_arg
    ) constexpr noexcept -> bool {
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
      [parent_args_tup = std::forward_as_tuple(args...
       )](const auto& new_arg) constexpr noexcept -> bool {
        return tuple::any_of(parent_args_tup, equal_to(new_arg));
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
  return predicate {
    [parent_arg = std::forward<T>(arg)](const auto& new_arg
    ) constexpr noexcept -> bool {
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
  return predicate {
    [parent_arg = std::forward<T>(arg)](const auto& new_arg
    ) constexpr noexcept -> bool {
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
  return predicate {
    [parent_arg = std::forward<T>(arg)](const auto& new_arg
    ) constexpr noexcept -> bool {
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
  return predicate {
    [parent_arg = std::forward<T>(arg)](const auto& new_arg
    ) constexpr noexcept -> bool {
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
  return predicate {
    [parent_arg = std::forward<T>(arg)](const auto& new_arg
    ) constexpr noexcept -> bool {
      return new_arg <= parent_arg;
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
      std::forward<Preds>(preds)...}}](auto&& arg
    ) constexpr noexcept -> bool {
      return std::apply(
        [&arg](auto&&... inner_preds) constexpr noexcept(
          noexcept((inner_preds(std::forward<decltype(arg)>(arg)) && ...))
        ) {
          return (inner_preds(std::forward<decltype(arg)>(arg)) && ...);
        },
        pred_tup
      );
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
      std::forward<Preds>(preds)...}}](auto&& arg
    ) constexpr noexcept -> bool {
      return std::apply(
        [&arg](auto&&... inner_preds) constexpr noexcept(
          noexcept((inner_preds(std::forward<decltype(arg)>(arg)) || ...))
        ) {
          return (inner_preds(std::forward<decltype(arg)>(arg)) || ...);
        },
        pred_tup
      );
    }};
}

/* {{{ doc */
/**
 * @brief Logical negation of a predicate
 *
 * @details Yes, this is redundant to `std::not_fn`,
 * this exists for naming consistency with the remaining
 * `pred_*` functions.
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
  return predicate {[inner_pred {std::forward<Pred>(pred)}](auto&& arg
                    ) constexpr noexcept {
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
[[nodiscard]] constexpr auto
pred_and(Pred1&& pred1, Pred2&& pred2) noexcept
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
[[nodiscard]] constexpr auto
pred_xor(Pred1&& pred1, Pred2&& pred2) noexcept
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
[[nodiscard]] constexpr auto
pred_implies(Pred1&& pred1, Pred2&& pred2) noexcept
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
[[nodiscard]] constexpr auto
pred_bicond(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return pred_not(
    pred_xor(std::forward<Pred1>(pred1), std::forward<Pred2>(pred2))
  );
}

namespace impl {
  template <typename T>
  struct is_predicate_impl : std::false_type { };

  template <typename T>
  struct is_predicate_impl<predicate<T>> : std::true_type { };

  template <typename T>
  struct is_predicate : is_predicate_impl<supl::remove_cvref_t<T>> { };

  template <typename T>
  constexpr inline bool is_predicate_v = is_predicate<T>::value;

}  // namespace impl

/* {{{ doc */
/**
 * @brief Logical negation of a predicate
 */
/* }}} */
template <
  typename Pred,
  typename = std::enable_if_t<impl::is_predicate_v<Pred>>>
[[nodiscard]] constexpr auto operator!(Pred&& pred) noexcept
{
  return pred_not(std::forward<Pred>(pred));
}

/* {{{ doc */
/**
 * @brief Logical conjunction (and) of two predicates
 */
/* }}} */
template <
  typename Pred1,
  typename Pred2,
  typename = std::enable_if_t<
    impl::is_predicate_v<Pred1> && impl::is_predicate_v<Pred2>>>
[[nodiscard]] constexpr auto
operator&&(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return pred_and(std::forward<Pred1>(pred1), std::forward<Pred2>(pred2));
}

/* {{{ doc */
/**
 * @brief Logical disjunction (inclusive or) of two predicates
 */
/* }}} */
template <
  typename Pred1,
  typename Pred2,
  typename = std::enable_if_t<
    impl::is_predicate_v<Pred1> && impl::is_predicate_v<Pred2>>>
[[nodiscard]] constexpr auto
operator||(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return pred_or(std::forward<Pred1>(pred1), std::forward<Pred2>(pred2));
}

/* {{{ doc */
/**
 * @brief Logical xor of two predicates
 */
/* }}} */
template <
  typename Pred1,
  typename Pred2,
  typename = std::enable_if_t<
    impl::is_predicate_v<Pred1> && impl::is_predicate_v<Pred2>>>
[[nodiscard]] constexpr auto
operator^(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return pred_xor(std::forward<Pred1>(pred1), std::forward<Pred2>(pred2));
}
}  // namespace supl

#endif
