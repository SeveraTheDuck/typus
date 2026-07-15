/**
 * @file back.hpp
 * @author SeveraTheDuck
 * @brief Combinator that extracts the last type from the pipeline.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/singleton.hpp>
#include <typus/model/thunk.hpp>

namespace typus {

namespace detail {

struct Back final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
    requires(sizeof...(Ts) > 0)
  struct Impl<base::Thunk<Ts...>> {
   private:
    static constexpr auto Size = sizeof...(Ts);

   public:
    using Type = base::Singleton<Ts...[Size - 1]>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Extracts the last element of the pipeline.
 *
 * The extracted type is wrapped in a `base::Singleton`. This combinator will
 * cause a compilation error if the pipeline is empty.
 *
 * @par Example
 * @code
 * using T = typus::Get<typus::From<int, float, double> | typus::Back>; // double
 * @endcode
 */
inline constexpr auto Back = detail::Back{};

}  // namespace typus
