/**
 * @file flatten.hpp
 * @author SeveraTheDuck
 * @brief Combinator that merges a pipeline of thunks into a single flat thunk.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

template <typename... Ts>
struct FlattenMany;

template <>
struct FlattenMany<> {
  using Type = base::Thunk<>;
};

template <typename... As>
struct FlattenMany<base::Thunk<As...>> {
  using Type = base::Thunk<As...>;
};

template <typename... As, typename... Bs, typename... Rest>
struct FlattenMany<base::Thunk<As...>, base::Thunk<Bs...>, Rest...> {
  using Type = FlattenMany<base::Thunk<As..., Bs...>, Rest...>::Type;
};

struct Flatten final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    static_assert(
        (model::Thunk<Ts> && ...),
        "typus::Flatten expects a Thunk containing only Thunks. "
        "Check if you missed a Map step or provided flat types.");

    using Type = FlattenMany<Ts...>::Type;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Merges one level of nested Thunks into a single, flat Thunk.
 *
 * Flattens exactly one level: every element of the input must itself be a
 * Thunk, and a mixed pipeline (some elements not Thunks) is a compile error,
 * not a partial pass. Deeper nesting is left untouched as in a monadic join.
 * Commonly used after a Map that produces multiple types per element (see FlatMap).
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<typus::base::Thunk<int>, typus::base::Thunk<float>>
 *                  | typus::Flatten;
 * // Result: Thunk<int, float>
 * @endcode
 */
inline constexpr auto Flatten = detail::Flatten{};

}  // namespace typus
