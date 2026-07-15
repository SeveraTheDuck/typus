/**
 * @file pipe_expr.hpp
 * @author SeveraTheDuck
 * @brief Defines the concept for any valid left-hand side expression in a pipeline.
 */
#pragma once

#include "anchored.hpp"
#include "combinator.hpp"

namespace typus::model {

/**
 * @brief Concept satisfied by types that can be piped from.
 *
 * A PipeExpr can be either data (Anchored) to be processed, or an incomplete
 * operation (Combinator) waiting to be composed with another operation.
 *
 * @tparam E The type to check.
 */
template <typename E>
concept PipeExpr = Anchored<E> || Combinator<E>;

}  // namespace typus::model
