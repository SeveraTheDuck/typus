/**
 * @file fn.hpp
 * @author SeveraTheDuck
 * @brief Combinator that applies a standard meta-function to a single type.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/normalize_naming.hpp>

namespace typus {

namespace detail {

template <template <typename> typename F>
struct Fn final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename T>
  struct Impl<base::Singleton<T>> {
    using Type = base::Singleton<typename NormalizeMetaFnType<F<T>>::Type>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Applies a unary template meta-function to a single wrapped type.
 *
 * Expects exactly one type in the pipeline (a Singleton). The meta-function
 * must expose a member type (either `::type` or `::Type`). Typically used
 * inside a `Map` combinator to transform multiple elements.
 *
 * @tparam F The unary template alias or struct (e.g., `std::add_pointer_t`).
 *
 * @par Example
 * @code
 * using Ptr = typus::Get<typus::From<int> | typus::Fn<std::add_pointer>>; // int*
 * @endcode
 */
template <template <typename> typename F>
inline constexpr auto Fn = detail::Fn<F>{};

}  // namespace typus
