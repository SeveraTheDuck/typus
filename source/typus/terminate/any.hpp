/**
 * @file any.hpp
 * @author SeveraTheDuck
 * @brief Eager terminator that checks if at least one element satisfies a predicate.
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
struct Any final : tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> : std::bool_constant<(Holds<Predicate, Ts> || ...)> {};

 public:
  template <model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

/**
 * @brief Evaluates whether at least one type in the pipeline satisfies the given predicate.
 *
 * This is an eager terminator. When piped into, it triggers evaluation and
 * returns a boolean value. Returns false for an empty pipeline.
 *
 * @tparam Predicate The combinator used as the condition for each type.
 */
template <model::Combinator auto Predicate>
inline constexpr auto Any = detail::Any<Predicate>{};

}  // namespace typus
