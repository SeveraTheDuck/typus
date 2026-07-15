#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/to_array.hpp>

#include <algorithm>

namespace typus {

namespace detail {

template <typename U>
struct Contains final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
   private:
    static constexpr auto types = ToArray<Ts...>();

   public:
    static constexpr bool value = std::ranges::contains(types, ^^U);
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

template <typename U>
inline constexpr auto Contains = detail::Contains<U>{};

}  // namespace typus
