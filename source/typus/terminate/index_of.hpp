/**
 * @file index_of.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that finds the index of a specific type.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/to_array.hpp>

#include <algorithm>
#include <cstddef>
#include <meta>
#include <type_traits>

namespace typus {

namespace detail {

template <typename U>
struct IndexOf final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
   private:
    static constexpr auto types = ToArray<Ts...>();
    static constexpr auto pos = std::ranges::find(types, ^^U);

   public:
    static constexpr auto value = static_cast<std::size_t>(pos - types.begin());
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Finds the zero-based index of the first occurrence of a type.
 *
 * This eager terminator evaluates the pipeline and returns the index. If the
 * type is not found, the returned index equals the size of the pipeline.
 *
 * @tparam U The type to search for.
 */
template <typename U>
inline constexpr auto IndexOf = detail::IndexOf<U>{};

}  // namespace typus
