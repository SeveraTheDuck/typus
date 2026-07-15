#pragma once

#include <concepts>

namespace typus::detail {

struct AnchorTag {};

template <typename T>
concept HasAnchorTag = std::derived_from<T, AnchorTag>;

}  // namespace typus::detail
