#pragma once

#include "thunk.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus::base {

template <model::Thunk /*Thunk*/>
struct TypeTuple;

template <typename... Ts>
struct TypeTuple<Thunk<Ts...>> final {
  template <template <typename...> typename Target>
  using As = Target<Ts...>;

  template <std::size_t N>
  using At = Ts...[N];

  static constexpr auto Size = sizeof...(Ts);
};

}  // namespace typus::base
