#pragma once

#include "at.hpp"

namespace typus {

namespace detail {

using Front = At<0>;

}  // namespace detail

inline constexpr auto Front = detail::Front{};

}  // namespace typus
