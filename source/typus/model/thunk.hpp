#pragma once

#include <typus/base/thunk.hpp>

#include <type_traits>

namespace typus::model {

namespace detail {

template <typename>
struct IsThunk : std::false_type {};

template <typename... Ts>
struct IsThunk<base::Thunk<Ts...>> : std::true_type {};

}  // namespace detail

template <typename T>
concept Thunk = detail::IsThunk<std::remove_cvref_t<T>>::value;

template <typename Op, typename T>
concept AppliesTo = Thunk<T> && requires { typename Op::template Apply<T>; };

}  // namespace typus::model
