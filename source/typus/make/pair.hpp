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
 * The value-form entry point for a pair; the pair is an ordinary two-element
 * Thunk, so it flows through the pipeline and composes with Flatten like any
 * other list.
 *
 * @tparam A The first type.
 * @tparam B The second type.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int> | typus::Concat<typus::Pair<float, double>>;
 * // Thunk<int, float, double>
 * @endcode
 */
template <typename A, typename B>
inline constexpr auto Pair = base::Pair<A, B>{};

}  // namespace typus
