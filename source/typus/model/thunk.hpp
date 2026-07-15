/**
 * @file thunk.hpp
 * @author SeveraTheDuck
 * @brief Defines the concept for the fundamental Thunk type.
 */
#pragma once

#include <typus/base/thunk.hpp>

#include <type_traits>

namespace typus::model {

namespace detail {

template <typename>
struct IsThunk : std::false_type {};

template <typename... Ts>
struct IsThunk<base::Thunk<Ts...>> : std::true_type {};

}  // namespace detail

/**
 * @brief Concept strictly satisfied by instantiations of typus::base::Thunk.
 *
 * Identifies the primary phantom types used to transport type lists
 * through the pipeline ecosystem.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept Thunk = detail::IsThunk<std::remove_cvref_t<T>>::value;

}  // namespace typus::model
