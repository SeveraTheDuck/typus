/**
 * @file tag.hpp
 * @author SeveraTheDuck
 * @brief Internal tags for AST node categorization.
 */
#pragma once

#include <concepts>
#include <type_traits>

namespace typus::detail {

struct AnchorTag {};

template <typename T>
concept HasAnchorTag = std::derived_from<std::remove_cvref_t<T>, AnchorTag>;

}  // namespace typus::detail
