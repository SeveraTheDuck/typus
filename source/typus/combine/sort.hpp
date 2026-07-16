/**
 * @file sort.hpp
 * @author SeveraTheDuck
 * @brief Combinator family for sorting types via C++26 reflection.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <algorithm>
#include <functional>
#include <meta>

namespace typus {

namespace detail {

template <auto Cmp, auto Proj>
struct SortBy final : tag::Combinator {
 private:
  template <typename... Ts>
  static consteval std::meta::info SortedInfo() {
    auto types = ToArray<Ts...>();
    std::ranges::sort(types, Cmp, Proj);
    return FromRange(std::move(types));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:SortedInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Sorts the pipeline using a custom comparator and projection.
 *
 * Functions provided must operate on `std::meta::info` at compile-time.
 * Order of projection-equal types is unspecified.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, char, double> | typus::SortBy<std::ranges::less{},
 *                                                                   std::meta::size_of>;
 * // Result: Thunk<char, int, double>
 * @endcode
 */
template <auto Cmp = std::ranges::less{}, auto Proj = std::identity{}>
inline constexpr auto SortBy = detail::SortBy<Cmp, Proj>{};

/**
 * @brief Sorts the pipeline by type size in ascending order.
 *
 * Order of projection-equal types is unspecified.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<double, char, int> | typus::SortBySize;
 * // Result: Thunk<char, int, double>
 * @endcode
 */
inline constexpr auto SortBySize = detail::SortBy<std::ranges::less{}, std::meta::size_of>{};

/**
 * @brief Sorts the pipeline by type size in descending order.
 *
 * Order of projection-equal types is unspecified.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<char, double, int> | typus::SortBySizeDec;
 * // Result: Thunk<double, int, char>
 * @endcode
 */
inline constexpr auto SortBySizeDec = detail::SortBy<std::ranges::greater{}, std::meta::size_of>{};

/**
 * @brief Sorts the pipeline by type alignment requirement in ascending order.
 *
 * Order of projection-equal types is unspecified.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<double, char> | typus::SortByAlignment;
 * // Result: Thunk<char, double>
 * @endcode
 */
inline constexpr auto SortByAlignment =
    detail::SortBy<std::ranges::less{}, std::meta::alignment_of>{};

}  // namespace typus
