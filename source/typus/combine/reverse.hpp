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

inline constexpr auto Reverse = detail::Reverse{};

}  // namespace typus
