/**
 * @file singleton.hpp
 * @author SeveraTheDuck
 * @brief Provides a utility to construct a Thunk containing exactly one type.
 */
#pragma once

#include <typus/base/singleton.hpp>

namespace typus {

/**
 * @brief Constructs a Thunk carrying a single type.
 *
 * This is a lightweight alternative to `From<T>` when explicitly dealing with
 * individual types. It acts as a pipeline anchor.
 *
 * @tparam T The type to wrap.
 *
 * @par Example
 * @code
 * constexpr auto single = typus::Singleton<int>;  // Thunk<int>
 * @endcode
 */
template <typename T>
inline constexpr auto Singleton = base::Singleton<T>{};

}  // namespace typus
