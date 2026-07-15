#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

struct Back final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(sizeof...(Ts) > 0)
  struct Impl<base::Thunk<Ts...>> {
   private:
    static constexpr auto Size = sizeof...(Ts);

   public:
    using Type = base::Singleton<Ts...[Size - 1]>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Back = detail::Back{};

}  // namespace typus
