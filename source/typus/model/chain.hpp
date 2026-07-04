#pragma once

#include <typus/base/chain.hpp>

#include <type_traits>

namespace typus::model {

namespace detail {

template <typename>
struct IsChain final : std::false_type {};

template <typename Lhs, typename Rhs>
struct IsChain<base::Chain<Lhs, Rhs>> final : std::true_type {};

}  // namespace detail

template <typename C>
concept Chain = detail::IsChain<std::remove_cvref_t<C>>::value;

}  // namespace typus::model
