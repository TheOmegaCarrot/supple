#ifndef SUPPLE_CORE_INVOKE_HPP
#define SUPPLE_CORE_INVOKE_HPP

#include <type_traits>
#include <utility>

namespace supl {

namespace impl {

  template <typename Invocable, typename Object, typename... Args>
  constexpr auto invoke_member_pointer(
    Invocable invocable,
    Object&& object,
    Args&&... args
  ) noexcept(std::is_nothrow_invocable_v<Invocable, Args...>)
    -> decltype(auto)
  {
    if constexpr ( std::is_member_function_pointer_v<Invocable> ) {
      if constexpr ( std::is_pointer_v<Object> ) {
        return (std::forward<Object>(object)->*invocable)(
          std::forward<Args>(args)...
        );
      } else {
        return (std::forward<Object>(object).*invocable)(
          std::forward<Args>(args)...
        );
      }
    } else /* is pointer to object */ {
      if constexpr ( std::is_pointer_v<Object> ) {
        return (std::forward<Object>(object))->*invocable;
      } else {
        return (std::forward<Object>(object)).*invocable;
      }
    }
  }

}  // namespace impl

/* {{{ doc */
/**
   * @brief Constexpr equivalant to `std::invoke`
   *
   * @details If this function differs in any way from `std::invoke` other
   * than being usable in a constexpr context, that is a bug.
   *
   * @bug Does not work directly with smart pointers
   * when using pointer-to-member.
   * **Non-ideal fix**: call `.get()` on it for the raw pointer,
   * and it will work.
   *
   */
/* }}} */
template <typename Invocable, typename... Args>
constexpr auto invoke(
  Invocable&& invocable,
  Args&&... args
) noexcept(std::is_nothrow_invocable_v<Invocable, Args...>)
  -> decltype(auto)
{
  if constexpr ( std::is_member_pointer_v<Invocable> ) {
    return impl::invoke_member_pointer(
      invocable, std::forward<Args>(args)...
    );
  } else {
    return invocable(std::forward<Args>(args)...);
  }
}

}  // namespace supl

#endif
