/**
 * @file bind.hpp
 * @author SeveraTheDuck
 * @brief Combinator that performs a Cartesian product with another pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/pair.hpp>
#include <typus/base/thunk.hpp>
#include <typus/model/anchored.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>
#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <array>
#include <meta>
#include <vector>

namespace typus {

namespace detail {

template <auto Other>
struct Bind final : tag::Combinator {
 private:
  template <typename, typename>
  struct Impl;

  template <typename... Lhs, typename... Rhs>
  struct Impl<base::Thunk<Lhs...>, base::Thunk<Rhs...>> {
   private:
    [[nodiscard]] static consteval std::meta::info MakeBind() {
      std::vector<std::meta::info> pairs;
      pairs.reserve(sizeof...(Lhs) * sizeof...(Rhs));

      auto lhs_arr = ToArray<Lhs...>();
      auto rhs_arr = ToArray<Rhs...>();

      for (auto l : lhs_arr) {
        for (auto r : rhs_arr) {
          std::array args{l, r};
          pairs.push_back(std::meta::substitute(^^base::Pair, args));
        }
      }

      return FromRange(std::move(pairs));
    }

   public:
    using Type = [:MakeBind():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T, Finalize<Other>>::Type;
};

}  // namespace detail

/**
 * @brief Combines each type in the current pipeline with every type in another pipeline.
 *
 * A type-level Cartesian product. The result is a pipeline of `base::Pair<L, R>`
 * elements (each pair being a two-element Thunk).
 *
 * @tparam Other The anchored pipeline to bind against.
 *
 * @par Example
 * @code
 * // Thunk<Pair<int, double>, Pair<int, char>>
 * constexpr auto p = typus::From<int> | typus::Bind<typus::From<double, char>>;
 * @endcode
 */
template <model::Anchored auto Other>
inline constexpr auto Bind = detail::Bind<Other>{};

}  // namespace typus
