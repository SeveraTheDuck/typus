#pragma once

#include "apply_to_one.hpp"

#include <typus/model/combinator.hpp>

namespace typus::detail {

template <model::Combinator auto Predicate, typename T>
inline constexpr bool Holds = NormalizeValue<ApplyToOneResult<Predicate, T>>::Value;

}  // namespace typus::detail
