#pragma once

#include "zip.hpp"

#include <typus/make/iota.hpp>

#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>

namespace typus {

namespace detail {

struct Enumerate final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> final {
    static constexpr auto Size = sizeof...(Ts);
    static constexpr auto Zipped = base::Thunk<Ts...>{} | typus::Zip<typus::Iota<Size>>;

    using Type = Finalize<Zipped>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Enumerate = detail::Enumerate{};

}  // namespace typus
