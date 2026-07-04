#pragma once

#include <typus/base/thunk.hpp>

namespace typus {

template <typename... Ts>
inline constexpr auto From = base::Thunk<Ts...>{};

}  // namespace typus
