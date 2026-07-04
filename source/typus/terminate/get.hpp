#pragma once

#include <typus/combine/fold.hpp>

namespace typus {

namespace detail {

template <typename>
struct Unwrap;

template <typename... Ts>
  requires(sizeof...(Ts) == 1)
struct Unwrap<base::Thunk<Ts...>> {
  using Type = Ts...[0];
};

}  // namespace detail

template <model::PipeExpr auto Pipeline>
using Get = detail::Unwrap<detail::Fold::Apply<decltype(Pipeline)>>::Type;

}  // namespace typus
