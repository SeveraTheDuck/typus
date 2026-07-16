/**
 * @file finalize.hpp
 * @author SeveraTheDuck
 * @brief Utility to resolve anchored pipelines into their resulting types.
 */
#pragma once

#include "forced.hpp"
#include "type_of.hpp"

#include <typus/model/anchored.hpp>

namespace typus::detail {

template <model::Anchored auto Pipeline>
using Finalize = Forced<TypeOf<Pipeline>>;

}  // namespace typus::detail
