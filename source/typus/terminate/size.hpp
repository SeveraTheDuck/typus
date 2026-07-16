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
 * This eager terminator expects the types in the pipeline to expose a static
 * constant value (via `::value` or `::Value`). It folds them using
 * addition. If the pipeline is empty, defaults to returning
 * `std::size_t{0}`.
 *
 * @par Example
 * @code
 * constexpr auto count = typus::From<int, float, double> | typus::Size; // 3
 * @endcode
 */
inline constexpr auto Size = detail::Size{};

}  // namespace typus
