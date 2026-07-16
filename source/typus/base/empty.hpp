/**
 * @file empty.hpp
 * @author SeveraTheDuck
 * @brief Provides a type alias for an empty Thunk.
 */
#pragma once

#include "thunk.hpp"

namespace typus::base {

/**
 * @brief An alias representing a Thunk with no elements.
 */
using Empty = Thunk<>;

}  // namespace typus::base
