#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/holds.hpp>

#include <type_traits>

namespace typus {

namespace detail {

template <auto Predicate>
struct All final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> : std::bool_constant<(Holds<Predicate, Ts> && ...)> {};

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

template <model::Combinator auto Predicate>
inline constexpr auto All = detail::All<Predicate>{};

}  // namespace typus
