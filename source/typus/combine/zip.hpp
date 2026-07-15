#pragma once

#include "tag.hpp"

#include <typus/base/pair.hpp>
#include <typus/base/thunk.hpp>

#include <typus/model/anchored.hpp>
#include <typus/model/thunk.hpp>

#include <algorithm>
#include <utility>

namespace typus {

namespace detail {

template <auto Other>
struct Zip final : tag::Combinator {
 private:
  template <typename, typename>
  struct Impl;

  template <typename... Lhs, typename... Rhs>
  struct Impl<base::Thunk<Lhs...>, base::Thunk<Rhs...>> {
    static constexpr auto Count = std::min(sizeof...(Lhs), sizeof...(Rhs));

    template <std::size_t... Is>
    static consteval auto Build(std::index_sequence<Is...> /*unused*/) noexcept {
      return base::Thunk<base::Pair<Lhs...[Is], Rhs...[Is]>...>{};
    }

    using Type = TypeOf<Build(std::make_index_sequence<Count>{})>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T, Finalize<Other>>::Type;
};

}  // namespace detail

template <model::Anchored auto Other>
inline constexpr auto Zip = detail::Zip<Other>{};

}  // namespace typus
