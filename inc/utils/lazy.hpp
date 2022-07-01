#ifndef EHANC_UTILS_LAZY_HPP
#define EHANC_UTILS_LAZY_HPP

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

namespace ehanc {

/* {{{ doc */
/**
 * @brief Lazy evaluator. Given a callable which takes no arguments,
 * this class will seamlessly and implicitly convert to the result
 * of calling the callable, but only call the callable a single time.
 *
 * @tparam RetType Return type of `m_func`. An `ehanc::lazy` is implicitly
 * convertible to the object of type `RetType` returned by `m_func`.
 *
 * @code
 *
 *	ehanc::lazy<int> example([](){return longComputation()});
 *
 *	// longComputation() only evaluated once,
 *	// and its return value is stored.
 *	use(example);
 *	use(example);
 *	use(example);
 *
 * @endcode
 */
/* }}} */
template <typename RetType>
class lazy
{
private:
  /* {{{ doc */
  /**
   * The callable which takes no arguments and returns
   * a useful value.
   */
  /* }}} */
  mutable std::function<RetType()> m_func;

  /* {{{ doc */
  /**
   * The stored result of calling `m_func`, if it has been
   * needed.
   */
  /* }}} */
  mutable std::optional<RetType> m_value;

public:
  /* {{{ doc */
  /**
   * Default constructor deleted to prevent
   * construction without a callable.
   */
  /* }}} */
  lazy() = delete;

  /* {{{ doc */
  /**
   * Enforce initialization with an appropriate callable.
   */
  /* }}} */
  template <typename Func>
  // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
  constexpr explicit lazy(Func&& func) noexcept
      : m_func{std::forward<Func>(func)}
      , m_value{}
  {}

  /* {{{ doc */
  /**
   * Weird to copy
   */
  /* }}} */
  constexpr lazy(const lazy& src) noexcept = delete;

  /* {{{ doc */
  /**
   * Weird to copy.
   */
  /* }}} */
  constexpr lazy& operator=(const lazy& rhs) noexcept = delete;

  /* {{{ doc */
  /**
   * Straightforward to move.
   */
  /* }}} */
  constexpr lazy(lazy&& src) noexcept = default;

  /* {{{ doc */
  /**
   * Straightforward to move.
   */
  /* }}} */
  constexpr lazy& operator=(lazy&& rhs) noexcept = default;

  /* {{{ doc */
  /**
   * Straightforward to destruct.
   */
  /* }}} */
  ~lazy() noexcept = default;

  /* {{{ doc */
  /**
   * @brief Type of contained value.
   */
  /* }}} */
  using type = RetType;

  /* {{{ doc */
  /**
   * @brief Provides access to contained callable.
   *
   * @return Contained callable.
   */
  /* }}} */
  [[nodiscard]] constexpr auto func() const noexcept
      -> std::add_lvalue_reference_t<std::function<RetType()>>
  {
    return m_func;
  }

  [[nodiscard]] constexpr bool has_value() const noexcept
  {
    return m_value.has_value();
  }

  /* {{{ doc */
  /**
   * @brief Allows direct read-only access to contained value.
   * If no value is contained, compute the value.
   *
   * @return Const reference to contained value.
   */
  /* }}} */
  [[nodiscard]] constexpr auto get() const noexcept(noexcept(m_func()))
      -> std::add_lvalue_reference_t<const RetType>
  {
    if ( not m_value.has_value() ) {
      m_value = m_func();
    }
    return *m_value;
  }

  /* {{{ doc */
  /**
   * @brief Allow for implicit conversion to the value returned
   * by `m_func`, but only ever call `m_func` once.
   *
   * @return Const reference to contained value.
   */
  /* }}} */
  [[nodiscard]] constexpr operator const RetType&() const
      noexcept(noexcept(m_func()))
  {
    return this->get();
  }
};

/* {{{ doc */
/**
 * @brief Helper function to deduce types for constructing
 * an ehanc::lazy where explicit instantiation would be impossible
 * or cumbersome.
 *
 * @note Number of use cases reduced dramatically after addition of
 * deduction guide. Kept mostly for cases where using this
 * improves clarity.
 *
 * @tparam Func Callable type taking no arguments and returning non-void.
 *
 * @param func Callable taking no arguments and returning non-void.
 *
 * @return `ehanc::lazy` object containing an object of the
 * return type of `func`.
 */
/* }}} */
template <typename Func>
constexpr auto make_lazy(Func&& func) noexcept
    -> lazy<std::invoke_result_t<Func>>
{
  return lazy<decltype(func())>(std::forward<Func>(func));
}

// deduction guide thanks to /u/i_lack_chromosomes on reddit!
// link to comment: https://www.reddit.com/r/cpp_questions/comments/vct7ct
template <typename Func>
lazy(Func) -> lazy<
    typename decltype(std::function(std::declval<Func>()))::result_type>;

namespace impl {
/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to determine
 * if a type is an ehanc::lazy.
 */
/* }}} */
template <typename T>
struct is_lazy_impl : std::false_type {};

/* {{{ doc */
/**
 * @brief Partial implementation of metafunction to determine
 * if a type is an ehanc::lazy. Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_lazy_impl<lazy<T>> : std::true_type {};
} // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type is an ehanc::lazy.
 */
/* }}} */
template <typename T>
struct is_lazy : impl::is_lazy_impl<std::decay_t<T>> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make ehanc::is_lazy
 * less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline const bool is_lazy_v = is_lazy<T>::value;

namespace impl {
/* {{{ doc */
/**
 * @brief Partial implementation of metafunction to determine
 * if the second parameter if an ehanc::lazy of the first parameter.
 */
/* }}} */
template <typename T, typename L>
struct is_lazy_of_impl : std::false_type {};

/* {{{ doc */
/**
 * @brief Partial implementation of metafunction to determine
 * if the second parameter if an ehanc::lazy of the first parameter.
 * Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_lazy_of_impl<T, lazy<T>> : std::true_type {};
} // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type is an ehanc::lazy.
 */
/* }}} */
template <typename T, typename L>
struct is_lazy_of : impl::is_lazy_of_impl<T, std::decay_t<L>> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make ehanc::is_lazy_of
 * less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename L>
constexpr inline const bool is_lazy_of_v = is_lazy_of<T, L>::value;

namespace impl {
/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to
 * retrieve the inner type of an ehanc::lazy;
 * identity if parameter is not an ehanc::lazy.
 */
/* }}} */
template <typename T>
struct lazy_inner_type_impl {
  using type = T;
};

/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to
 * retrieve the inner type of an ehanc::lazy;
 * identity if parameter is not an ehanc::lazy.
 * Specialization for case where type is an ehanc::lazy.
 */
/* }}} */
template <typename T>
struct lazy_inner_type_impl<lazy<T>> {
  using type = T;
};
} // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to retrieve the inner type of an ehanc::lazy;
 * identity if parameter is not an ehanc::lazy.
 */
/* }}} */
template <typename T>
struct lazy_inner_type : impl::lazy_inner_type_impl<std::decay_t<T>> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `lazy_inner_type`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
using lazy_inner_type_t = typename lazy_inner_type<T>::type;

} // namespace ehanc

#endif
