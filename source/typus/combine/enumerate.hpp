/**
 * @file enumerate.hpp
 * @author SeveraTheDuck
 * @brief Combinator that pairs each type with its zero-based index.
 */
#pragma once

#include "zip.hpp"

#include <typus/make/iota.hpp>

#include <typus/model/thunk.hpp>

#include <typus/details/finalize.hpp>

namespace typus {

namespace detail {

struct Enumerate final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> final {
    static constexpr auto Size = sizeof...(Ts);
    static constexpr auto Zipped = base::Thunk<Ts...>{} | typus::Zip<typus::Iota<Size>>;

    using Type = Finalize<Zipped>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Zips each element in the pipeline with its index.
 *
 * The resulting pipeline contains `base::Pair<T, Index>` elements, where
 * `Index` is an `std::integral_constant<std::size_t, N>`.
 *
 * @par Example
 * @code
 * // Thunk<Pair<int, ic<0>>, Pair<float, ic<1>>>
 * constexpr auto p = typus::From<int, float> | typus::Enumerate;
 * @endcode
 */
inline constexpr auto Enumerate = detail::Enumerate{};

}  // namespace typus
