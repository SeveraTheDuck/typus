/**
 * @file from.hpp
 * @author SeveraTheDuck
 * @brief Provides a utility to construct a Thunk from a variadic pack of types.
 */
#pragma once

#include <typus/base/thunk.hpp>

namespace typus {

/**
 * @brief Constructs a Thunk carrying the specified types.
 *
 * Acts as the primary entry point for type-level pipelines,
 * lifting raw C++ types into the Typus evaluation model.
 *
 * @tparam Ts The types to wrap inside the Thunk.
 *
 * @par Example
 * @code
 * constexpr auto types = typus::From<int, float, double>;
 * @endcode
 */
template <typename... Ts>
inline constexpr auto From = base::Thunk<Ts...>{};

}  // namespace typus
