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

template <template <typename> typename F>
inline constexpr auto Fn = detail::Fn<F>{};

}  // namespace typus
