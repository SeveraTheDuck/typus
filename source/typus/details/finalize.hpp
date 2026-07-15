#pragma once

#include "forced.hpp"
#include "type_of.hpp"

#include <typus/model/anchored.hpp>

namespace typus::detail {

template <model::Anchored auto Pipeline>
using Finalize = Forced<TypeOf<Pipeline>>;

}  // namespace typus::detail
