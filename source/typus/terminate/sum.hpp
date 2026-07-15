/**
 * @file sum.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that calculates the sum of numeric type-level values.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/normalize_naming.hpp>

namespace typus {

namespace detail {

struct Sum final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(sizeof...(Ts) > 0)
  struct Impl<base::Thunk<Ts...>> {
    static constexpr auto value = (NormalizeValue<Ts>::Value + ...);
  };

  template <typename... Ts>
    requires(sizeof...(Ts) == 0)
  struct Impl<base::Thunk<Ts...>> {
    static constexpr std::size_t value = 0;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Adds the numeric values exposed by the types in the pipeline.
 *
 * The return type is automatically deduced from the values being summed.
 * If the pipeline is empty, defaults to returning `std::size_t{0}`.
 *
 * @par Example
 * @code
 * using V2 = std::integral_constant<int, 2>;
 * using V3 = std::integral_constant<int, 3>;
 * constexpr auto total = typus::From<V2, V3> | typus::Sum; // 5
 * @endcode
 */
inline constexpr auto Sum = detail::Sum{};

}  // namespace typus
