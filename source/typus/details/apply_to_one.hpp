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
