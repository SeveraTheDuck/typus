#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <typename... Us>
struct Append final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<Ts..., Us...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

template <typename... Us>
inline constexpr auto Append = detail::Append<Us...>{};

}  // namespace typus
