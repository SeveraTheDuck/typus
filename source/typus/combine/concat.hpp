#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/anchored.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>

namespace typus {

namespace detail {

template <auto Other>
struct Concat final : tag::Combinator {
 private:
  template <typename, typename>
  struct Impl;

  template <typename... Ls, typename... Rs>
  struct Impl<base::Thunk<Ls...>, base::Thunk<Rs...>> {
    using Type = base::Thunk<Ls..., Rs...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T, Finalize<Other>>::Type;
};

}  // namespace detail

/// From<A, B> | Concat<From<C, D>>  ->  Thunk<A, B, C, D>
template <model::Anchored auto Other>
inline constexpr auto Concat = detail::Concat<Other>{};

}  // namespace typus
