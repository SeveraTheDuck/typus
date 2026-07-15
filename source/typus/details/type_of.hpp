#pragma once

#include <type_traits>

namespace typus::detail {

template <auto V>
using TypeOf = std::remove_cvref_t<decltype(V)>;

}  // namespace typus::detail
