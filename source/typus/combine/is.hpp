/**
 * @file is.hpp
 * @author SeveraTheDuck
 * @brief Adapts a type-trait to a Typus combinator.
 */
#pragma once

#include "tag.hpp"

#include <typus/details/normalize_naming.hpp>

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
 * @brief Adapts a type-trait to a Typus combinator.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::Map<typus::Is<std::is_const>>;
 * @endcode
 */
template <template <typename> typename P>
inline constexpr auto Is = detail::Is<P>{};

}  // namespace typus
