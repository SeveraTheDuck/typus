/**
 * @file combinator.hpp
 * @author SeveraTheDuck
 * @brief Defines the concept for lazy pipeline operations.
 */
#pragma once

#include <typus/combine/tag.hpp>

#include <concepts>
#include <type_traits>

namespace typus::model {

/**
 * @brief Concept satisfied by types acting as lazy combinators.
 *
 * A Combinator represents an intermediate transformation step in a pipeline.
 * It must derive from `tag::Combinator` to participate in lazy evaluation
 * via `operator|`.
 *
 * @tparam C The type to check.
 */
template <typename C>
concept Combinator = std::derived_from<std::remove_cvref_t<C>, tag::Combinator>;

}  // namespace typus::model
