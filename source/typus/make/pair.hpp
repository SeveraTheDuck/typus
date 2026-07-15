/**
 * @file pair.hpp
 * @author SeveraTheDuck
 * @brief Provides a utility to construct a Thunk containing a pair of types.
 */
#pragma once

#include <typus/base/pair.hpp>

namespace typus {

/**
 * @brief Constructs a Thunk carrying a pair of types.
 *
 * @tparam A The first type.
 * @tparam B The second type.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::Pair<int, float>; // Pair<int, float>
 * @endcode
 */
template <typename A, typename B>
inline constexpr auto Pair = base::Pair<A, B>{};

}  // namespace typus
