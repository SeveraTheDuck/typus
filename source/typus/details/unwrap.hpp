/**
 * @file unwrap.hpp
 * @author SeveraTheDuck
 * @brief Extracts the underlying type from a Singleton.
 */
#pragma once

#include <typus/base/singleton.hpp>

namespace typus::detail {

template <typename>
struct Unwrap;

template <typename T>
struct Unwrap<base::Singleton<T>> final {
  using Type = T;
};

}  // namespace typus::detail
