/**
 * @file not.hpp
 * @author SeveraTheDuck
 * @brief Logical NOT combinator.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/normalize_naming.hpp>

#include <type_traits>

namespace typus {

namespace detail {
struct Not final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename B>
    requires HasSnakeCaseValue<B> or HasCamelCaseValue<B>
  struct Impl<base::Singleton<B>> {
    using Type = base::Singleton<std::bool_constant<not NormalizeValue<B>::Value>>;
  };

  template <typename B>
  struct Impl<base::Singleton<B>> {
    static_assert(
        HasSnakeCaseValue<B> or HasCamelCaseValue<B>,
        "typus::Not expects a single boolean result (a type with ::value / "
        "::Value), but received a raw element. Most likely the negation is "
        "on the wrong side of the pipe: write `Is<P> | Not`, not "
        "`Not | Is<P>` - Not inverts the *result* of a predicate.");
  };

  template <typename... Bs>
    requires(sizeof...(Bs) != 1)
  struct Impl<base::Thunk<Bs...>> {
    static_assert(
        sizeof...(Bs) == 1,
        "typus::Not expects exactly one boolean result, but the incoming "
        "Thunk carries several elements. Not belongs after a predicate "
        "producing a single verdict (e.g. `Is<P> | Not`), not after a "
        "list-producing stage.");
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Logical inversion of a predicate result.
 *
 * Not is postfix: it inverts the verdict already produced upstream, so it
 * belongs after a predicate (`Is<P> | Not`), never before one.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::Map<typus::Is<std::is_integral> | typus::Not>;
 * @endcode
 */
inline constexpr auto Not = detail::Not{};

}  // namespace typus
