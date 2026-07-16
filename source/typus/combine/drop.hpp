/**
 * @file drop.hpp
 * @author SeveraTheDuck
 * @brief Combinator that removes the first N elements from the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <algorithm>
#include <cstddef>
#include <meta>
#include <span>

namespace typus {

namespace detail {

template <std::size_t N>
struct Drop final : tag::Combinator {
 private:
  template <typename... Ts>
  static consteval std::meta::info DroppedInfo() {
    constexpr auto Skip = std::min(N, sizeof...(Ts));
    auto types = ToArray<Ts...>();
    return FromRange(std::span{types}.subspan(Skip));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:DroppedInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Discards the first N types from the pipeline.
 *
 * If N is greater than or equal to the total number of elements, the resulting
 * pipeline will be empty (a zero-element Thunk).
 *
 * @tparam N The number of elements to drop.
 *
 * @par Example
 * @code
 * // Thunk<float, double>
 * constexpr auto p = typus::From<int, float, double> | typus::Drop<1>;
 * @endcode
 */
template <std::size_t N>
inline constexpr auto Drop = detail::Drop<N>{};

}  // namespace typus
