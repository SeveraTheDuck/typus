/**
 * @file iota.hpp
 * @author SeveraTheDuck
 * @brief Generates a sequence of compile-time indices.
 */
#pragma once

#include <typus/base/thunk.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace typus {

namespace detail {

template <std::size_t N>
consteval auto Iota() noexcept {
  constexpr auto maker = []<std::size_t... Is>(std::index_sequence<Is...>) consteval noexcept {
    return base::Thunk<std::integral_constant<std::size_t, Is>...>{};
  };

  return maker(std::make_index_sequence<N>{});
}

}  // namespace detail

/**
 * @brief Generates a Thunk containing a sequence of sizes from 0 to N-1.
 *
 * The generated types are strictly `std::integral_constant<std::size_t, I>`.
 * Useful for index-based operations like Zip or mapping over element positions.
 *
 * @tparam N The number of index elements to generate.
 *
 * @par Example
 * @code
 * // Generates Thunk<std::integral_constant<std::size_t, 0>,
 * //                 std::integral_constant<std::size_t, 1>>
 * constexpr auto indices = typus::Iota<2>;
 * @endcode
 */
template <std::size_t N>
inline constexpr auto Iota = detail::Iota<N>();

}  // namespace typus
