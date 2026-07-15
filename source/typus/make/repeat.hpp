#pragma once

#include <typus/base/thunk.hpp>

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

template <typename T, std::size_t N>
inline constexpr auto Repeat = detail::Repeat<T, N>();

}  // namespace typus
