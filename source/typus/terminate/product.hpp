#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/normalize_naming.hpp>

namespace typus {

namespace detail {

struct Product final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(sizeof...(Ts) > 0)
  struct Impl<base::Thunk<Ts...>> {
    static constexpr auto value = (NormalizeValue<Ts>::Value * ...);
  };

  template <typename... Ts>
    requires(sizeof...(Ts) == 0)
  struct Impl<base::Thunk<Ts...>> {
    static constexpr std::size_t value = 1;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

inline constexpr auto Product = detail::Product{};

}  // namespace typus
