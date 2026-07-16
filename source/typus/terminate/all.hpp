/**
 * @file all.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that checks if all elements satisfy a predicate.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/holds.hpp>

#include <type_traits>

namespace typus {

namespace detail {

template <auto Predicate>
struct All final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> : std::bool_constant<(Holds<Predicate, Ts> && ...)> {};

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Evaluates whether all types in the pipeline satisfy the given predicate.
 *
 * This is an eager terminator. When piped into, it triggers evaluation and
 * returns a boolean value. Returns true for an empty pipeline.
 *
 * @tparam Predicate The combinator used as the condition for each type.
 *
 * @par Example
 * @code
 * constexpr bool res = typus::From<int, int> | typus::All<typus::Is<std::is_integral>>; // true
 * @endcode
 */
template <model::Combinator auto Predicate>
inline constexpr auto All = detail::All<Predicate>{};

}  // namespace typus
