#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/anchored.hpp>

#include <typus/details/chain/chain.hpp>

namespace typus {

namespace detail {

struct Force final : tag::Combinator {
 private:
  template <typename /*T*/>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<Ts...>;
  };

  template <typename Lhs, typename Rhs>
  struct Impl<Chain<Lhs, Rhs>> {
    using Type = Rhs::template Apply<typename Impl<Lhs>::Type>;
  };

 public:
  template <model::Anchored T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Force = detail::Force{};

}  // namespace typus
