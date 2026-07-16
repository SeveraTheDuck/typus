/**
 * @file reverse.hpp
 * @author SeveraTheDuck
 * @brief Combinator that reverses the order of types in the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <algorithm>

namespace typus {

namespace detail {

struct Reverse final : tag::Combinator {
 private:
  template <typename... Ts>
  [[nodiscard]] static consteval std::meta::info ReversedInfo() {
    auto types = ToArray<Ts...>();
    std::ranges::reverse(types);
    return FromRange(std::move(types));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> final {
    using Type = [:ReversedInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Reverses the sequence of types currently in the pipeline.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, float, double> | typus::Reverse;
 * // Result: Thunk<double, float, int>
 * @endcode
 */
inline constexpr auto Reverse = detail::Reverse{};

}  // namespace typus
