// typus/combine/sort.hpp
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <algorithm>
#include <array>
#include <meta>

namespace typus {

namespace detail {

template <auto Proj>
struct SortBy final : tag::Combinator {
 private:
  template <typename... Ts>
  static consteval std::meta::info SortedInfo() {
    std::array<std::meta::info, sizeof...(Ts)> types{^^Ts...};
    std::ranges::sort(types, {}, Proj);   // Proj: info → ordered value
    return std::meta::substitute(^^base::Thunk, types);
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:SortedInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

template <auto Proj>
inline constexpr auto Sort = detail::SortBy<Proj>{};

inline constexpr auto SortBySize = detail::SortBy<std::meta::size_of>{};
inline constexpr auto SortByAlignment = detail::SortBy<std::meta::alignment_of>{};

}  // namespace typus
