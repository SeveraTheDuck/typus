#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <typus/details/holds.hpp>

#include <meta>
#include <vector>

namespace typus {

namespace detail {

template <model::Combinator auto Predicate>
struct Find final : tag::Combinator {
 private:
  template <typename... Ts>
  [[nodiscard]] static consteval std::meta::info FoundInfo() {
    std::vector<std::meta::info> kept;

    template for (constexpr auto t : ToArray<Ts...>()) {
      if constexpr (Holds<Predicate, typename[:t:]>) {
        kept.push_back(t);
        break;
      }
    }

    return FromRange(std::move(kept));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:FoundInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

template <model::Combinator auto Predicate>
inline constexpr auto Find = detail::Find<Predicate>{};

}  // namespace typus
