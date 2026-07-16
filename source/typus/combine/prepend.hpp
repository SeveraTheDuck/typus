/**
 * @file prepend.hpp
 * @author SeveraTheDuck
 * @brief Combinator that adds new types to the beginning of the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <typename... Us>
struct Prepend final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<Us..., Ts...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Inserts the specified types at the front of the current pipeline.
 *
 * @tparam Us The types to prepend.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<float, double> | typus::Prepend<int>;
 * // Result: Thunk<int, float, double>
 * @endcode
 */
template <typename... Us>
inline constexpr auto Prepend = detail::Prepend<Us...>{};

}  // namespace typus
