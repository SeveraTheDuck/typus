#pragma once

#include "combinator.hpp"
#include "terminator.hpp"

namespace typus::model {

template <typename Op>
concept Operation = Combinator<Op> || Terminator<Op>;

}  // namespace typus::model
