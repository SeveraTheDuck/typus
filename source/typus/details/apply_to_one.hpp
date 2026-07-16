/**
 * @file apply_to_one.hpp
 * @author SeveraTheDuck
 * @brief Applies a pipeline to one type: the shared element-level primitive.
 *
 * Builds Singleton<T> | Pipeline and forces it. This is the common mechanism
 * behind Map (per-element transform) and predicates (Holds); the primitive
 * itself is agnostic to what the result means.
 */
#pragma once

#include "finalize.hpp"
#include "unwrap.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/combinator.hpp>

namespace typus::detail {

template <model::Combinator auto Pipeline, typename T>
using ApplyToOne = Finalize<base::Singleton<T>{} | Pipeline>;

template <model::Combinator auto Pipeline, typename T>
using ApplyToOneResult = Unwrap<ApplyToOne<Pipeline, T>>::Type;

}  // namespace typus::detail
