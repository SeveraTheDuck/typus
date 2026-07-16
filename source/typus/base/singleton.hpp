/**
 * @file singleton.hpp
 * @author SeveraTheDuck
 * @brief Provides a type alias for a single-element Thunk.
 */
#pragma once

#include "thunk.hpp"

namespace typus::base {

/**
 * @brief An alias representing a Thunk containing exactly one type.
 *
 * @tparam T The type contained in the Thunk.
 */
template <typename T>
using Singleton = Thunk<T>;

}  // namespace typus::base
