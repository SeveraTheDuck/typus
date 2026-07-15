#pragma once

#include "at.hpp"

namespace typus {

namespace detail {

using First = At<0>;
using Second = At<1>;

}  // namespace detail

inline constexpr auto First = detail::First{};
inline constexpr auto Second = detail::Second{};

}  // namespace typus
