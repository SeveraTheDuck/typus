/**
 * @file zip.hpp
 * @author SeveraTheDuck
 * @brief Combinator that zips two pipelines into a pipeline of pairs.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/pair.hpp>
#include <typus/base/thunk.hpp>

#include <typus/model/anchored.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>
#include <typus/details/type_of.hpp>

#include <algorithm>
#include <cstddef>
#include <utility>

namespace typus {

namespace detail {

template <auto Other>
struct Zip final : tag::Combinator {
 private:
  template <typename, typename>
  struct Impl;

  template <typename... Lhs, typename... Rhs>
  struct Impl<base::Thunk<Lhs...>, base::Thunk<Rhs...>> {
    static constexpr auto Count = std::min(sizeof...(Lhs), sizeof...(Rhs));

    template <std::size_t... Is>
    static consteval auto Build(std::index_sequence<Is...> /*unused*/) noexcept {
      return base::Thunk<base::Pair<Lhs...[Is], Rhs...[Is]>...>{};
    }

    using Type = TypeOf<Build(std::make_index_sequence<Count>{})>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T, Finalize<Other>>::Type;
};

}  // namespace detail

/**
 * @brief Pairs types from two pipelines index-wise.
 *
 * Stops at the length of the shorter pipeline.
 *
 * @tparam Other The anchored pipeline to zip with.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, double> | typus::Zip<typus::From<float, char>>;
 * // Result: Thunk<Pair<int, float>, Pair<double, char>>
 * @endcode
 */
template <model::Anchored auto Other>
inline constexpr auto Zip = detail::Zip<Other>{};

}  // namespace typus
