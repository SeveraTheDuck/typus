/**
 * @file model.hpp
 * @author SeveraTheDuck
 * @brief Internal concepts for detecting and categorizing Chains.
 */
#pragma once

#include "tag.hpp"

#include <typus/model/combinator.hpp>

#include <type_traits>

namespace typus::detail {

template <typename Lhs, typename Rhs>
struct Chain;

namespace impl {

template <typename>
struct IsChain final : std::false_type {};

template <typename Lhs, typename Rhs>
struct IsChain<Chain<Lhs, Rhs>> final : std::true_type {};

}  // namespace impl

template <typename C>
concept IsChain = impl::IsChain<std::remove_cvref_t<C>>::value;

template <typename C>
concept AnchoredChain = IsChain<C> && HasAnchorTag<C>;

}  // namespace typus::detail
