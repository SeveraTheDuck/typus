#pragma once

#include "flatten.hpp"
#include "tag.hpp"

#include <typus/base/pipe.hpp>
#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>

namespace typus {

namespace detail {

template <typename T>
struct ExtractTypes {
 private:
  template <typename U>
  static constexpr bool AlwaysFalse = false;

 public:
  static_assert(
      AlwaysFalse<T>,
      "typus::Unpack encountered a type that is either not a template "
      "or contains non-type template parameters. Only templates "
      "instantiated with types (e.g., std::tuple<A, B>) are supported.");
};

template <template <typename...> class Templ, typename... Args>
struct ExtractTypes<Templ<Args...>> {
  using Type = base::Thunk<Args...>;
};

struct Unpack final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Extracted = base::Thunk<typename ExtractTypes<Ts>::Type...>;
    static constexpr auto Flattened = Extracted{} | Flatten{};

    using Type = Finalize<Flattened>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto Unpack = detail::Unpack{};

}  // namespace typus
