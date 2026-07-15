#pragma once

#include "thunk.hpp"

namespace typus::base {

template <typename T>
using Singleton = Thunk<T>;

}  // namespace typus::base
