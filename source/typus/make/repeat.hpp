/**
 * @file repeat.hpp
 * @author SeveraTheDuck
 * @brief Generates a Thunk with a type repeated multiple times.
 */
#pragma once

#include <typus/base/thunk.hpp>

#include <array>
#include <cstddef>
#include <meta>

namespace typus {

namespace detail {

template <typename T, std::size_t N>
consteval auto Repeat() noexcept {
  constexpr auto info = [] consteval {
    std::array<std::meta::info, N> types;
    types.fill(^^T);
    return std::meta::substitute(^^base::Thunk, types);
  }();

  return typename[:info:]{};
}

}  // namespace detail

/**
 * @brief Generates a Thunk containing the specified type repeated N times.
 *
 * Uses C++26 reflection to avoid recursive template instantiations.
 *
 * @tparam T The type to repeat.
 * @tparam N The number of times to repeat the type.
 *
 * @par Example
 * @code
 * // Generates Thunk<char, char, char, char>
 * constexpr auto padding = typus::Repeat<char, 4>;
 * @endcode
 */
template <typename T, std::size_t N>
inline constexpr auto Repeat = detail::Repeat<T, N>();

}  // namespace typus
