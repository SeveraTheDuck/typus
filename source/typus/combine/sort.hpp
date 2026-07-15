// typus/combine/sort.hpp
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <algorithm>
#include <meta>

namespace typus {

namespace detail {

template <auto Cmp, auto Proj>
struct SortBy final : tag::Combinator {
 private:
  template <typename... Ts>
  static consteval std::meta::info SortedInfo() {
    auto types = ToArray<Ts...>();
    std::ranges::sort(types, Cmp, Proj);
    return FromRange(std::move(types));
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

template <auto Cmp = std::ranges::less{}, auto Proj = std::identity{}>
inline constexpr auto SortBy = detail::SortBy<Cmp, Proj>{};

inline constexpr auto SortBySize = detail::SortBy<std::ranges::less{}, std::meta::size_of>{};

inline constexpr auto SortBySizeDec = detail::SortBy<std::ranges::greater{}, std::meta::size_of>{};

inline constexpr auto SortByAlignment =
    detail::SortBy<std::ranges::less{}, std::meta::alignment_of>{};

}  // namespace typus
