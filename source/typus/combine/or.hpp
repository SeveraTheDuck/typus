/**
 * @file or.hpp
 * @author SeveraTheDuck
 * @brief Logical OR combinator.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/holds.hpp>

#include <type_traits>

namespace typus {

namespace detail {

template <model::Combinator auto Lhs, model::Combinator auto Rhs>
struct Or final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename T>
  struct Impl<base::Singleton<T>> {
    using Type = base::Singleton<std::bool_constant<Holds<Lhs, T> or Holds<Rhs, T>>>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Logical OR of two predicate results.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::Map<typus::Or<typus::Is<std::is_integral>,
 *                                         typus::Is<std::is_floating_point>>>;
 * @endcode
 */
template <model::Combinator auto Lhs, model::Combinator auto Rhs>
inline constexpr auto Or = detail::Or<Lhs, Rhs>{};

}  // namespace typus
