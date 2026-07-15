#pragma once

#include "thunk.hpp"

namespace typus::base {

template <typename A, typename B>
using Pair = Thunk<A, B>;

}  // namespace typus::base
