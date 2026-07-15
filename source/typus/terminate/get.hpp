/**
 * @file get.hpp
 * @author SeveraTheDuck
 * @brief Utility alias to evaluate a pipeline and extract a single type.
 */
#pragma once

#include <typus/details/finalize.hpp>
#include <typus/details/unwrap.hpp>
#include <typus/model/anchored.hpp>

namespace typus {

/**
 * @brief Forces a pipeline evaluation and unwraps the resulting Singleton.
 *
 * Useful for pipelines designed to isolate exactly one type, such as those
 * ending with `At<N>`, `Front`, or `Back`.
 *
 * @tparam Pipeline The anchored pipeline expression to evaluate.
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::From<int, float> | typus::At<1>>; // float
 * @endcode
 */
template <model::Anchored auto Pipeline>
using Get = detail::Unwrap<detail::Finalize<Pipeline>>::Type;

}  // namespace typus
