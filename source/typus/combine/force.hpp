/**
 * @file force.hpp
 * @author SeveraTheDuck
 * @brief Core execution engine that folds the AST and forces pipeline evaluation.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/anchored.hpp>

#include <typus/details/chain/chain.hpp>

namespace typus {

namespace detail {

struct Force final : tag::Combinator {
 private:
  template <typename /*T*/>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<Ts...>;
  };

  template <typename Lhs, typename Rhs>
  struct Impl<Chain<Lhs, Rhs>> {
    using Type = Rhs::template Apply<typename Impl<Lhs>::Type>;
  };

 public:
  template <model::Anchored T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Eagerly evaluates a composed AST Chain.
 *
 * Recursively traverses the pipeline nodes, passing intermediate types from
 * the left-hand side into the right-hand side operations. Used internally by
 * eager utilities like `Get`, `Materialize`, and other terminators.
 */
inline constexpr auto Force = detail::Force{};

}  // namespace typus
