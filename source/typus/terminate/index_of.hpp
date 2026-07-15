#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/to_array.hpp>

#include <algorithm>
#include <cstddef>
#include <type_traits>

namespace typus {

namespace detail {

template <typename U>
struct IndexOf final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
   private:
    static constexpr auto types = ToArray<Ts...>();
    static constexpr auto pos = std::ranges::find(types, ^^U);

   public:
    static constexpr auto value = static_cast<std::size_t>(pos - types.begin());
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

template <typename U>
inline constexpr auto IndexOf = detail::IndexOf<U>{};

}  // namespace typus
