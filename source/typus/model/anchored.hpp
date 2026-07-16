/**
 * @file anchored.hpp
 * @author SeveraTheDuck
 * @brief Defines the concept for types that hold evaluateable data in a pipeline.
 */
#pragma once

#include "thunk.hpp"

#include <typus/details/chain/model.hpp>

namespace typus::model {

/**
 * @brief Concept satisfied by types that act as data anchors.
 *
 * An Anchored type is either a raw data carrier (like a Thunk) or a pipeline
 * Chain that begins with a Thunk. These are the types that can be materialized
 * or eagerly evaluated when piped into a Terminator.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept Anchored = Thunk<T> || typus::detail::AnchoredChain<T>;

}  // namespace typus::model
