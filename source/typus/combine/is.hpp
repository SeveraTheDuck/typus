/**
 * @file is.hpp
 * @author SeveraTheDuck
 * @brief Adapts a type-trait to a Typus combinator.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

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

}  // namespace detail

/**
 * @brief Adapts a unary type-trait into a predicate combinator.
 *
 * Wraps the trait's ::value / ::Value verdict into a Singleton of
 * std::bool_constant, making any standard or user trait pipeable.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::Map<typus::Is<std::is_const>>;
 * @endcode
 */
template <template <typename> typename P>
inline constexpr auto Is = detail::Is<P>{};

}  // namespace typus
