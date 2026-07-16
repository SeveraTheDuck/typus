/**
 * @file pair.hpp
 * @author SeveraTheDuck
 * @brief Semantic aliases for extracting elements from base::Pair.
 */
#pragma once

#include "at.hpp"

namespace typus {

namespace detail {

using First = At<0>;
using Second = At<1>;

}  // namespace detail

/**
 * @brief Extracts the first element of a pair (or any Thunk).
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::Pair<int, double> | typus::First>; // int
 * @endcode
 */
inline constexpr auto First = detail::First{};

/**
 * @brief Extracts the second element of a pair (or any Thunk).
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::Pair<int, double> | typus::Second>; // double
 * @endcode
 */
inline constexpr auto Second = detail::Second{};

}  // namespace typus
