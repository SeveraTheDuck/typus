/**
 * @file contains.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that checks if a specific type exists in the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/to_array.hpp>

#include <algorithm>

namespace typus {

namespace detail {

template <typename U>
struct Contains final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
   private:
    static constexpr auto types = ToArray<Ts...>();

   public:
    static constexpr bool value = std::ranges::contains(types, ^^U);
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Evaluates whether the pipeline contains the specified type.
 *
 * This is an eager terminator that utilizes C++26 reflection to perform
 * the search without recursive template instantiation.
 *
 * @tparam U The type to search for.
 */
template <typename U>
inline constexpr auto Contains = detail::Contains<U>{};

}  // namespace typus
