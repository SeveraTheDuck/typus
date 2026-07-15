/**
 * @file front.hpp
 * @author SeveraTheDuck
 * @brief Utility combinator to extract the first type from the pipeline.
 */
#pragma once

#include "at.hpp"

namespace typus {

namespace detail {

using Front = At<0>;

}  // namespace detail

/**
 * @brief Extracts the very first element of the pipeline.
 *
 * The extracted type is wrapped in a `base::Singleton`. This combinator will
 * cause a compilation error if the pipeline is empty.
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::From<int, float> | typus::Front>; // int
 * @endcode
 */
inline constexpr auto Front = detail::Front{};

}  // namespace typus
