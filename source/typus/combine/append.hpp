/**
 * @file append.hpp
 * @author SeveraTheDuck
 * @brief Combinator that appends new types to the end of the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <typename... Us>
struct Append final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<Ts..., Us...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Appends the specified types to the end of the current pipeline.
 *
 * @tparam Us The types to append.
 *
 * @par Example
 * @code
 * // Thunk<int, float, double>
 * constexpr auto p = typus::From<int> | typus::Append<float, double>;
 * @endcode
 */
template <typename... Us>
inline constexpr auto Append = detail::Append<Us...>{};

}  // namespace typus
