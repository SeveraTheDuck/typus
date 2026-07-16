/**
 * @file forced.hpp
 * @author SeveraTheDuck
 * @brief Internal utility to apply the Force combinator to an anchored type.
 */
#pragma once

#include "type_of.hpp"

#include <typus/combine/force.hpp>
#include <typus/model/anchored.hpp>

namespace typus::detail {

template <model::Anchored Pipeline>
using Forced = Force::Apply<Pipeline>;

}  // namespace typus::detail
