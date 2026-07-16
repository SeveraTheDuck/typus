/**
 * @file at.hpp
 * @author SeveraTheDuck
 * @brief Combinator that extracts a type at a specific index.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/thunk.hpp>

#include <cstddef>

namespace typus {

namespace detail {

template <std::size_t N>
struct At final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(N < sizeof...(Ts))
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Singleton<Ts...[N]>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Extracts the element at the specified zero-based index.
 *
 * The extracted type is wrapped in a `base::Singleton`. This combinator will
 * cause a compilation error if the index is out of bounds.
 *
 * @tparam N The index of the type to extract.
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::From<int, float, double> | typus::At<1>>; // float
 * @endcode
 */
template <std::size_t N>
inline constexpr auto At = detail::At<N>{};

}  // namespace typus
