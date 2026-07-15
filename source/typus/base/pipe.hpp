#pragma once

#include <typus/model/anchored.hpp>
#include <typus/model/operation.hpp>
#include <typus/model/pipe_expr.hpp>
#include <typus/model/terminator.hpp>

#include <typus/details/chain/chain.hpp>
#include <typus/details/forced.hpp>
#include <typus/details/normalize_naming.hpp>

namespace typus::tag {  // god forbid ADL

// Lazy, composes chains of evaluation
template <model::PipeExpr Lhs, model::Operation Rhs>
[[nodiscard]] consteval auto operator|(Lhs, Rhs) noexcept -> detail::Chain<Lhs, Rhs> {
  return {};
}

// Eager, triggers only when value and terminator present
template <model::Anchored Lhs, model::Terminator Rhs>
[[nodiscard]] consteval auto operator|(Lhs, Rhs) {
  using Pipeline = detail::Chain<Lhs, Rhs>;
  using Applied = detail::Forced<Pipeline>;
  return detail::NormalizeValue<Applied>::Value;
}

}  // namespace typus::tag
