/**
 * @file type_of.hpp
 * @author SeveraTheDuck
 * @brief Shorthand for extracting the clean type of auto value.
 */
#pragma once

#include <type_traits>

namespace typus::detail {

template <auto V>
using TypeOf = std::remove_cvref_t<decltype(V)>;

}  // namespace typus::detail
