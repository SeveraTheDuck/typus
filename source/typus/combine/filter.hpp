/**
 * @file filter.hpp
 * @author SeveraTheDuck
 * @brief Combinator that filters elements based on a type-level predicate.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <typus/details/holds.hpp>

#include <meta>
#include <vector>

namespace typus {

namespace detail {

template <model::Combinator auto Predicate>
struct Filter final : tag::Combinator {
 private:
  template <typename... Ts>
  [[nodiscard]] static consteval std::meta::info FilteredInfo() {
    std::vector<std::meta::info> kept;
    kept.reserve(sizeof...(Ts));

    template for (constexpr auto t : ToArray<Ts...>()) {
      if constexpr (Holds<Predicate, typename[:t:]>) {
        kept.push_back(t);
      }
    }

    return FromRange(std::move(kept));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:FilteredInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Keeps only the types in the pipeline that satisfy the given predicate.
 *
 * Uses C++26 reflection to evaluate the predicate efficiently without
 * recursive template instantiations.
 *
 * @tparam Predicate The combinator used as the filtering condition.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, float, int> | typus::Filter<typus::Is<std::is_integral>>;
 * // Result: Thunk<int, int>
 * @endcode
 */
template <model::Combinator auto Predicate>
inline constexpr auto Filter = detail::Filter<Predicate>{};

}  // namespace typus
