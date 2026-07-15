#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <std::size_t N>
struct At final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(N < sizeof...(Ts))
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Singleton<Ts...[N]>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

template <std::size_t N>
inline constexpr auto At = detail::At<N>{};

}  // namespace typus
