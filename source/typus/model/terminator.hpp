/**
 * @file terminator.hpp
 * @author SeveraTheDuck
 * @brief Defines the concept for eager pipeline terminators.
 */
#pragma once

#include <typus/terminate/tag.hpp>

#include <concepts>
#include <type_traits>

namespace typus::model {

/**
 * @brief Concept satisfied by types that trigger pipeline evaluation.
 *
 * A Terminator extracts a final computed value (like a boolean or an integer)
 * from an Anchored pipeline. It must derive from `tag::ValueTerminator`.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept Terminator = std::derived_from<std::remove_cvref_t<T>, tag::ValueTerminator>;

}  // namespace typus::model
