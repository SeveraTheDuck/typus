/**
 * @file to_array.hpp
 * @author SeveraTheDuck
 * @brief Reflection utility to convert variadic packs into std::array of meta::info.
 */
#pragma once

#include <array>
#include <meta>

namespace typus::detail {

template <typename... Ts>
[[nodiscard]] consteval auto ToArray() noexcept {
  return std::array<std::meta::info, sizeof...(Ts)>{^^Ts...};
}

}  // namespace typus::detail
