/**
 * @file find.hpp
 * @author SeveraTheDuck
 * @brief Combinator that searches for the first element satisfying a predicate.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <typus/details/holds.hpp>

#include <array>
#include <cstddef>
#include <meta>

namespace typus {

namespace detail {

template <model::Combinator auto Predicate>
struct Find final : tag::Combinator {
 private:
  template <typename... Ts>
  [[nodiscard]] static consteval std::meta::info FoundInfo() {
    using None = std::array<std::meta::info, 0>;
    static constexpr auto Size = sizeof...(Ts);

    if constexpr (Size == 0) {
      return FromRange(None{});
    }

    static constexpr std::array matches = {Holds<Predicate, Ts>...};
    static constexpr auto types = ToArray<Ts...>();

    for (std::size_t i = 0; i < Size; ++i) {
      if (matches[i]) {
        std::array kept{types[i]};
        return FromRange(kept);
      }
    }

    return FromRange(None{});
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:FoundInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Extracts the first type in the pipeline that satisfies the predicate.
 *
 * If a match is found, returns a Thunk containing exactly that one type.
 * If no match is found, returns an empty Thunk.
 *
 * @tparam Predicate The combinator used as the search condition.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<float, int, double> | typus::Find<typus::Is<std::is_integral>>;
 * // Result: Thunk<int>
 * @endcode
 */
template <model::Combinator auto Predicate>
inline constexpr auto Find = detail::Find<Predicate>{};

}  // namespace typus
