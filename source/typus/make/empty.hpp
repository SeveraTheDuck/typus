/**
 * @file empty.hpp
 * @author SeveraTheDuck
 * @brief Provides an empty type-level thunk.
 */
#pragma once

#include <typus/base/thunk.hpp>

namespace typus {

/**
 * @brief An empty pipeline thunk containing no types.
 *
 * Useful as a starting point for operations that build up types incrementally,
 * or to explicitly represent the absence of types in a pipeline.
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::Empty | typus::Append<int>>; // int
 * @endcode
 */
inline constexpr auto Empty = base::Thunk{};

}  // namespace typus
