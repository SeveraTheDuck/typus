/**
 * @file flat_map.hpp
 * @author SeveraTheDuck
 * @brief Combinator that maps each element and flattens the resulting thunks.
 */
#pragma once

#include "flatten.hpp"
#include "map.hpp"

namespace typus {

/**
 * @brief Applies a pipeline to each element, then flattens the result.
 *
 * This is a composite pipeline combining `Map` and `Flatten`. The provided
 * pipeline MUST map each element to a `base::Thunk` (otherwise `Flatten` will assert).
 *
 * @tparam Pipeline The combinator/pipeline applied to each type.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<NodeA, NodeB> | typus::FlatMap<GetChildren>;
 * @endcode
 */
template <model::Combinator auto Pipeline>
inline constexpr auto FlatMap = Map<Pipeline> | Flatten;

}  // namespace typus
