#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <template <typename> typename F>
struct Map final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<typename F<Ts>::type...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * Usage: typus::From<int, double, char> | typus::Map<std::add_pointer>
 */
template <template <typename> typename F>
inline constexpr auto Map = detail::Map<F>{};

}  // namespace typus
