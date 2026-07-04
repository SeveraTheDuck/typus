#pragma once

#include "chain.hpp"

#include <typus/model/combinator.hpp>
#include <typus/model/pipe_expr.hpp>
#include <typus/model/terminator.hpp>

#include <typus/combine/fold.hpp>

namespace typus::base {

template <model::PipeExpr Lhs, model::Combinator Rhs>
consteval auto operator|(Lhs, Rhs) -> Chain<Lhs, Rhs> {
  return {};
}

template <model::PipeExpr Lhs, model::Terminator Rhs>
consteval auto operator|(Lhs, Rhs) {
  using Pipeline = Chain<Lhs, Rhs>;
  return detail::Fold::Apply<Pipeline>::value;
}

}  // namespace typus::base
