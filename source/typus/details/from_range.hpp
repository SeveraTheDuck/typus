/**
 * @file from_range.hpp
 * @author SeveraTheDuck
 * @brief Reflection utility to convert ranges of meta::info into Thunks.
 */
#pragma once

#include <typus/base/thunk.hpp>

#include <meta>
#include <ranges>
#include <utility>

namespace typus::detail {

template <std::ranges::input_range R>
  requires std::same_as<std::ranges::range_value_t<R>, std::meta::info>
[[nodiscard]] consteval std::meta::info FromRange(R&& types) noexcept {
  return std::meta::substitute(^^base::Thunk, std::forward<R>(types));
}

}  // namespace typus::detail
