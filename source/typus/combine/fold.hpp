#pragma once

#include "tag.hpp"

#include <typus/model/pipe_expr.hpp>

namespace typus {

namespace detail {

struct Fold final : tag::Combinator {
 private:
  template <typename /*T*/>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<Ts...>;
  };

  template <typename Lhs, typename Rhs>
  struct Impl<base::Chain<Lhs, Rhs>> {
    using Type = Rhs::template Apply<typename Impl<Lhs>::Type>;
  };

 public:
  template <model::PipeExpr T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Fold = detail::Fold{};

}  // namespace typus
