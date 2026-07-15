#pragma once

#include <typus/base/singleton.hpp>

namespace typus {

template <typename T>
inline constexpr auto Singleton = base::Singleton<T>{};

}  // namespace typus
