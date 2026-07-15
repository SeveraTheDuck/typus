/**
 * @file size.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that calculates the number of elements in the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <cstddef>
#include <type_traits>

namespace typus {

namespace detail {

struct Size final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Returns the total number of types currently held in the pipeline.
 *
 * This eager terminator forces the evaluation of the AST and returns a
 * compile-time integer representing the count of the resulting types.
 */
inline constexpr auto Size = detail::Size{};

}  // namespace typus
