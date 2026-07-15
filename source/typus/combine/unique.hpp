#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <algorithm>
#include <meta>
#include <vector>

namespace typus {

namespace detail {

struct Unique final : tag::Combinator {
 private:
  template <typename... Ts>
  static consteval std::meta::info UniqueInfo() {
    std::vector<std::meta::info> kept;
    kept.reserve(sizeof...(Ts));

    for (auto t : ToArray<Ts...>()) {
      if (not std::ranges::contains(kept, t)) {
        kept.push_back(t);
      }
    }
    return FromRange(kept);
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:UniqueInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Unique = detail::Unique{};

}  // namespace typus
