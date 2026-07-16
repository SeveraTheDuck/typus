/**
 * @file pair.hpp
 * @author SeveraTheDuck
 * @brief Provides a type alias for a two-element Thunk.
 */
#pragma once

#include "thunk.hpp"

namespace typus::base {

/**
 * @brief An alias representing a Thunk containing exactly two types.
 *
 * Pairs are ordinary two-element Thunks, not a distinct type. This is
 * deliberate: the result of Zip composes directly with Flatten, since a
 * pipeline of pairs is a pipeline of Thunks.
 *
 * @tparam A The first type.
 * @tparam B The second type.
 */
template <typename A, typename B>
using Pair = Thunk<A, B>;

}  // namespace typus::base
