#pragma once

#include <array>
#include <meta>

namespace typus::detail {

template <typename... Ts>
[[nodiscard]] consteval auto ToArray() noexcept {
  return std::array{^^Ts...};
}

}  // namespace typus::detail
