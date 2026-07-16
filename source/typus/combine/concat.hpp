/**
 * @file concat.hpp
 * @author SeveraTheDuck
 * @brief Combinator that concatenates two pipelines.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/anchored.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>

namespace typus {

namespace detail {

template <auto Other>
struct Concat final : tag::Combinator {
 private:
  template <typename, typename>
  struct Impl;

  template <typename... Ls, typename... Rs>
  struct Impl<base::Thunk<Ls...>, base::Thunk<Rs...>> {
    using Type = base::Thunk<Ls..., Rs...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T, Finalize<Other>>::Type;
};

}  // namespace detail

/**
 * @brief Appends the results of another pipeline to the end of the current one.
 *
 * Example: `From<A, B> | Concat<From<C, D>>` evaluates to `Thunk<A, B, C, D>`.
 *
 * @tparam Other The anchored pipeline whose types should be concatenated.
 *
 * @par Example
 * @code
 * // Thunk<int, float, double>
 * constexpr auto p = typus::From<int> | typus::Concat<typus::From<float, double>>;
 * @endcode
 */
template <model::Anchored auto Other>
inline constexpr auto Concat = detail::Concat<Other>{};

}  // namespace typus
