#pragma once

#include "thunk.hpp"

#include <typus/details/chain/model.hpp>

namespace typus::model {

template <typename T>
concept Anchored = Thunk<T> || typus::detail::AnchoredChain<T>;

}  // namespace typus::model
