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

template <std::size_t N>
inline constexpr auto Iota = detail::Iota<N>();

}  // namespace typus
