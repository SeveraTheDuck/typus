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

template <std::size_t N>
inline constexpr auto Take = detail::Take<N>{};

}  // namespace typus
