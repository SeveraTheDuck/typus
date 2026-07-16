/**
 * @file materialize.hpp
 * @author SeveraTheDuck
 * @brief Utility alias to resolve a pipeline into an interfaceable TypeTuple.
 */
#pragma once

#include <typus/base/type_tuple.hpp>
#include <typus/model/anchored.hpp>

#include <typus/details/finalize.hpp>

namespace typus {

/**
 * @brief Forces pipeline evaluation and exposes the result through a TypeTuple.
 *
 * Serves as the primary bridge between the lazy type-list ecosystem and
 * the application code, allowing interaction with the final types via `As<Target>`
 * or `At<N>`.
 *
 * @tparam Pipeline The anchored pipeline expression to evaluate.
 *
 * @par Example
 * @code
 * using Tuple = typus::Materialize<typus::From<int, float> | typus::Reverse>;
 * using T = Tuple::At<0>; // float
 * @endcode
 */
template <model::Anchored auto Pipeline>
using Materialize = base::TypeTuple<detail::Finalize<Pipeline>>;

}  // namespace typus
