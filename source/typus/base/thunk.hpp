/**
 * @file thunk.hpp
 * @author SeveraTheDuck
 * @brief The fundamental type-list container of the library.
 */
#pragma once

namespace typus::base {

/**
 * @brief A lightweight, compile-time container for a variadic pack of types.
 *
 * The primary data carrier of the library: the normal form that combinators
 * consume and produce. A phantom type with no runtime state, existing only to
 * hold and transfer type sequences during compile-time evaluation.
 *
 * @tparam Ts The parameter pack of types.
 */
template <typename... /*Ts*/>
struct [[nodiscard]] Thunk final {};

}  // namespace typus::base
