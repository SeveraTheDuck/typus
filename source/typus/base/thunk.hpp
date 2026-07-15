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
 * Thunk serves as the primary data carrier throughout the type-level pipelines.
 * It is designed as a phantom type (having no runtime state) specifically
 * for holding and transferring type sequences during compile-time evaluation.
 *
 * @tparam Ts The parameter pack of types.
 */
template <typename... /*Ts*/>
struct [[nodiscard]] Thunk final {};

}  // namespace typus::base
