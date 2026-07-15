#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/holds.hpp>
#include <typus/details/normalize_naming.hpp>

#include <type_traits>

namespace typus {

namespace detail {

template <template <typename> typename P>
struct Is final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename T>
  struct Impl<base::Thunk<T>> {
    using Type = base::Thunk<std::bool_constant<NormalizeValue<P<T>>::Value>>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

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

template <auto Lhs, auto Rhs, typename T>
struct BoolBinOpImpl {
  static constexpr bool LhsResult = Holds<Lhs, T>;
  static constexpr bool RhsResult = Holds<Rhs, T>;
};

template <auto Lhs, auto Rhs>
struct And final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename T>
  struct Impl<base::Singleton<T>> {
    using Results = BoolBinOpImpl<Lhs, Rhs, T>;
    using Type = base::Singleton<std::bool_constant<Results::LhsResult and Results::RhsResult>>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

template <auto Lhs, auto Rhs>
struct Or final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename T>
  struct Impl<base::Singleton<T>> {
    using Results = BoolBinOpImpl<Lhs, Rhs, T>;
    using Type = base::Singleton<std::bool_constant<Results::LhsResult or Results::RhsResult>>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

template <template <typename> typename P>
inline constexpr auto Is = detail::Is<P>{};

inline constexpr auto Not = detail::Not{};

template <auto Lhs, auto Rhs>
inline constexpr auto And = detail::And<Lhs, Rhs>{};

template <auto Lhs, auto Rhs>
inline constexpr auto Or = detail::Or<Lhs, Rhs>{};

}  // namespace typus
