/**
 * @file pipe.hpp
 * @author SeveraTheDuck
 * @brief Core pipeline composition operators.
 */
#pragma once

#include <typus/model/anchored.hpp>
#include <typus/model/operation.hpp>
#include <typus/model/pipe_expr.hpp>
#include <typus/model/terminator.hpp>

#include <typus/details/chain/chain.hpp>
#include <typus/details/forced.hpp>
#include <typus/details/normalize_naming.hpp>

namespace typus::tag {  // god forbid ADL

/**
 * @brief Composes a lazy pipeline chain.
 *
 * This operator builds the Abstract Syntax Tree (AST) of the type-level
 * pipeline without evaluating it.
 *
 * @tparam Lhs The left-hand side expression (either an anchored Thunk or a Combinator).
 * @tparam Rhs The right-hand side operation (Combinator or Terminator).
 * @return A detail::Chain representing the composed pipeline AST.
 */
template <model::PipeExpr Lhs, model::Operation Rhs>
[[nodiscard]] consteval auto operator|(Lhs, Rhs) noexcept -> detail::Chain<Lhs, Rhs> {
  return {};
}

/**
 * @brief Eagerly evaluates a pipeline.
 *
 * This operator triggers when an anchored pipeline (containing actual type data)
 * meets a Terminator operation. It forces the evaluation of the entire pipeline
 * and returns the resulting value.
 *
 * @tparam Lhs The left-hand side anchored pipeline.
 * @tparam Rhs The right-hand side Terminator.
 * @return The computed value of the pipeline evaluation.
 */
template <model::Anchored Lhs, model::Terminator Rhs>
[[nodiscard]] consteval auto operator|(Lhs, Rhs) {
  using Pipeline = detail::Chain<Lhs, Rhs>;
  using Applied = detail::Forced<Pipeline>;
  return detail::NormalizeValue<Applied>::Value;
}

}  // namespace typus::tag
