/**
 * @file unpack.hpp
 * @author SeveraTheDuck
 * @brief Combinator that unpacks nested template arguments into the pipeline.
 */
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

/**
 * @brief Extracts types from template instantiations (e.g., std::tuple<A, B> -> A, B).
 *
 * Expects the pipeline to contain template instantiations. Unpacks the nested
 * arguments into a flat, single-level pipeline.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<std::tuple<int, float>> | typus::Unpack;
 * // Result: Thunk<int, float>
 * @endcode
 */
inline constexpr auto Unpack = detail::Unpack{};

}  // namespace typus
