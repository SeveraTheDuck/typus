/**
 * @file take.hpp
 * @author SeveraTheDuck
 * @brief Combinator that takes the first N elements from the pipeline.
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
struct Take final : tag::Combinator {
 private:
  template <typename... Ts>
  static consteval std::meta::info TakenInfo() {
    constexpr auto kCount = std::min(N, sizeof...(Ts));
    auto types = ToArray<Ts...>();
    return FromRange(std::span{types}.first(kCount));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:TakenInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Discards all but the first N types from the pipeline.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, float, double> | typus::Take<2>;
 * // Result: Thunk<int, float>
 * @endcode
 */
template <std::size_t N>
inline constexpr auto Take = detail::Take<N>{};

}  // namespace typus
