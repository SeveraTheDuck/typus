#pragma once

#include "flatten.hpp"
#include "map.hpp"

namespace typus {

template <model::Combinator auto Pipeline>
inline constexpr auto FlatMap = Map<Pipeline> | Flatten;

}  // namespace typus
