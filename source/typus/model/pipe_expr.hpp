#pragma once

#include "anchored.hpp"
#include "combinator.hpp"

namespace typus::model {

template <typename E>
concept PipeExpr = Anchored<E> || Combinator<E>;

}  // namespace typus::model
