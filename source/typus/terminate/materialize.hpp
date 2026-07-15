#pragma once

#include <typus/base/type_tuple.hpp>

#include <typus/model/anchored.hpp>

namespace typus {

template <model::Anchored auto Pipeline>
using Materialize = base::TypeTuple<detail::Finalize<Pipeline>>;

}  // namespace typus
