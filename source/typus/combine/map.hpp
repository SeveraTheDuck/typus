#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/apply_to_one.hpp>

namespace typus {

namespace detail {

template <auto Pipeline>
struct Map final : tag::Combinator {
 private:
  template <typename T>
  using MapOne = ApplyToOneResult<Pipeline, T>;

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<MapOne<Ts>...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * Usage: typus::From<int&, double>
 *        | typus::Map
 *          <
 *            typus::Fn<std::remove_cvref> |
 *            typus::Fn<std::add_pointer>
 *          >
 */
template <model::Combinator auto Pipeline>
inline constexpr auto Map = detail::Map<Pipeline>{};

}  // namespace typus
