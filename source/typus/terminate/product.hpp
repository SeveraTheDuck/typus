/**
 * @file product.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that calculates the product of numeric type-level values.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/normalize_naming.hpp>

namespace typus {

namespace detail {

struct Product final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(sizeof...(Ts) > 0)
  struct Impl<base::Thunk<Ts...>> {
    static constexpr auto value = (NormalizeValue<Ts>::Value * ...);
  };

  template <typename... Ts>
    requires(sizeof...(Ts) == 0)
  struct Impl<base::Thunk<Ts...>> {
    static constexpr std::size_t value = 1;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Multiplies the numeric values exposed by the types in the pipeline.
 *
 * This eager terminator expects the types in the pipeline to expose a static
 * constant value (via `::value` or `::Value`). It folds them using multiplication.
 */
inline constexpr auto Product = detail::Product{};

}  // namespace typus
