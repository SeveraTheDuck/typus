#pragma once

#include "at.hpp"

namespace typus {

namespace detail {

using Front = At<0>;

}

inline constexpr auto Front = detail::Front{};

}  // namespace typus
