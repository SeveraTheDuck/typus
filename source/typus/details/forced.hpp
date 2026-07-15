#pragma once

#include "type_of.hpp"

#include <typus/combine/force.hpp>
#include <typus/model/anchored.hpp>

namespace typus::detail {

template <model::Anchored Pipeline>
using Forced = Force::Apply<Pipeline>;

}  // namespace typus::detail
