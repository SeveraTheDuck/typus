/**
 * @file chain.hpp
 * @author SeveraTheDuck
 * @brief Core AST node for lazy pipeline composition.
 */
#pragma once

#include "tag.hpp"

#include <typus/model/anchored.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/operation.hpp>
#include <typus/model/pipe_expr.hpp>
#include <typus/model/terminator.hpp>
#include <typus/model/thunk.hpp>

#include <type_traits>

namespace typus::detail {

// clang-format off
template <typename Lhs, typename Rhs>
using ChainBase = std::conditional_t
<
    model::Anchored<Lhs>,
    AnchorTag,
    std::conditional_t
    <
      model::Terminator<Rhs>,
      tag::ValueTerminator,
      tag::Combinator
    >
>;
// clang-format on

/**
 * @brief Lazy composition node of the pipeline AST. Library-internal.
 *
 * Chain is the only product of `operator|`: it records "apply @p Rhs after
 * @p Lhs" without evaluating anything. Nesting of chains forms the whole
 * pipeline; evaluation is performed exclusively by typus::Force.
 *
 * The role of a chain is decided structurally (via ChainBase) and expressed
 * through its base tag:
 *
 * 1. **Anchored**: @p Lhs carries data (a Thunk, or recursively an anchored
 *    chain). The chain is a *value*: it can be evaluated by Force / Get /
 *    Materialize, but Apply is constrained away.
 * 2. **Combinator**: @p Lhs is itself a combinator, so no data is bound yet.
 *    The chain is a *function* over Thunks: applying it pipes the input
 *    through @p Lhs, then @p Rhs. Being a combinator by base, it composes
 *    further with `operator|` and fits any combinator slot (e.g. Filter's
 *    predicate).
 * 3. **ValueTerminator**: like 2, but @p Rhs terminates: the chain is a
 *    function from a Thunk to a value-bearing type. Meeting anchored data
 *    on `operator|` triggers eager evaluation.
 *
 * @tparam Lhs Head: previously composed pipeline prefix: a Thunk anchor,
 *             a combinator, or a nested Chain of either kind.
 * @tparam Rhs Tail: the operation recorded last: a combinator or a
 *             terminator.
 */
template <typename Lhs, typename Rhs>
struct [[nodiscard]] Chain final : ChainBase<Lhs, Rhs> {
 private:
  static_assert(
      model::PipeExpr<Lhs>,
      "typus: Chain head must be a Thunk anchor, a combinator, or another chain");

  template <typename T>
  struct Impl {
    using Type = Rhs::template Apply<typename Lhs::template Apply<T>>;
  };

 public:
  template <model::Thunk T>
    requires model::Operation<Chain>
  using Apply = Impl<T>::Type;
};

}  // namespace typus::detail
