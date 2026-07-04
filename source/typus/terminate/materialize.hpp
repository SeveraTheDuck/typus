#pragma once

#include <typus/base/type_tuple.hpp>

#include <typus/combine/fold.hpp>

#include <typus/model/pipe_expr.hpp>

namespace typus {

template <model::PipeExpr auto Pipeline>
using Materialize = base::TypeTuple<detail::Fold::Apply<decltype(Pipeline)>>;

}  // namespace typus
