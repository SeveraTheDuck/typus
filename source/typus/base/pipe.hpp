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
 * Builds the pipeline AST without evaluating it. The role of the resulting
 * Chain (anchored value vs. combinator) is decided structurally from its
 * head; this operator does not force evaluation.
 *
 * @tparam Lhs The left-hand side expression: anchored data (Thunk or anchored
 *             chain) or a Combinator being composed point-free.
 * @tparam Rhs A Combinator or Terminator to record next.
 * @return A detail::Chain node representing the composed AST.
 */
template <model::PipeExpr Lhs, model::Operation Rhs>
[[nodiscard]] consteval auto operator|(Lhs, Rhs) noexcept -> detail::Chain<Lhs, Rhs> {
  return {};
}

/**
 * @brief Eagerly evaluates an anchored pipeline against a terminator.
 *
 * Triggers only when anchored data meets a Terminator: forces the whole
 * pipeline and returns the resulting value. A combinator left-hand side does
 * not match here: terminating a data-less function is not composition.
 *
 * @tparam Lhs The anchored pipeline carrying data.
 * @tparam Rhs The terminating operation.
 * @return The computed value, read through ::value / ::Value normalization.
 */
template <model::Anchored Lhs, model::Terminator Rhs>
[[nodiscard]] consteval auto operator|(Lhs, Rhs) {
  using Pipeline = detail::Chain<Lhs, Rhs>;
  using Applied = detail::Forced<Pipeline>;
  return detail::NormalizeValue<Applied>::Value;
}

}  // namespace typus::tag
