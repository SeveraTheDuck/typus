#pragma once

#include <typus/details/unwrap.hpp>
#include <typus/details/finalize.hpp>

namespace typus {

template <model::Anchored auto Pipeline>
using Get = detail::Unwrap<detail::Finalize<Pipeline>>::Type;

}  // namespace typus
