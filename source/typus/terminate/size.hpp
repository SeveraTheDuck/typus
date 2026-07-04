#pragma once

#include "tag.hpp"

#include <typus/model/thunk.hpp>
#include <typus/base/thunk.hpp>

#include <type_traits>

namespace typus {

namespace detail {

struct Size final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

inline constexpr auto Size = detail::Size{};

}  // namespace typus
