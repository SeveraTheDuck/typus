/**
 * @file operation.hpp
 * @author SeveraTheDuck
 * @brief Defines the overarching concept for all pipeline operations.
 */
#pragma once

#include "combinator.hpp"
#include "terminator.hpp"

namespace typus::model {

/**
 * @brief Concept satisfied by any valid right-hand side pipeline operation.
 *
 * An Operation can be either a lazy transformation (Combinator) or
 * an eager evaluation trigger (Terminator).
 *
 * @tparam Op The type to check.
 */
template <typename Op>
concept Operation = Combinator<Op> || Terminator<Op>;

}  // namespace typus::model
