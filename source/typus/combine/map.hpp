/**
 * @file map.hpp
 * @author SeveraTheDuck
 * @brief Combinator that applies a pipeline to each element individually.
 */
#pragma once

#include "tag.hpp"

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
 * @brief Transforms each type in the pipeline by applying another pipeline to it.
 *
 * @tparam Pipeline The combinator pipeline to apply to each element.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int&, double>
 *                  | typus::Map<typus::Fn<std::remove_reference> | typus::Fn<std::add_pointer>>;
 * // Result: Thunk<int*, double*>
 * @endcode
 */
template <model::Combinator auto Pipeline>
inline constexpr auto Map = detail::Map<Pipeline>{};

}  // namespace typus
