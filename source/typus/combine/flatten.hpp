#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <typename... Ts>
struct FlattenMany;

template <>
struct FlattenMany<> {
  using Type = base::Thunk<>;
};

template <typename... As>
struct FlattenMany<base::Thunk<As...>> {
  using Type = base::Thunk<As...>;
};

template <typename... As, typename... Bs, typename... Rest>
struct FlattenMany<base::Thunk<As...>, base::Thunk<Bs...>, Rest...> {
  using Type = FlattenMany<base::Thunk<As..., Bs...>, Rest...>::Type;
};

struct Flatten final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    static_assert(
        (model::Thunk<Ts> && ...),
        "typus::Flatten expects a Thunk containing only Thunks. "
        "Check if you missed a Map step or provided flat types.");

    using Type = FlattenMany<Ts...>::Type;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Flatten = detail::Flatten{};

}  // namespace typus
