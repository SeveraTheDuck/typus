#pragma once

#include "chain.hpp"
#include "thunk.hpp"

namespace typus::model {

template <typename T>
concept PipeExpr = Thunk<T> || Chain<T>;

}
