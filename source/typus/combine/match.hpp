#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/holds.hpp>

#include <algorithm>
#include <array>
#include <cstddef>

namespace typus {

template <model::Combinator auto Predicate, typename R>
struct Case final {
  template <typename T>
  static constexpr bool Matches = detail::Holds<Predicate, T>;

  using Type = R;
};

template <typename R>
struct Default final {
  template <typename>
  static constexpr bool Matches = true;

  using Type = R;
};

namespace detail {

template <typename C, typename T>
concept MatchCase = requires {
  { C::template Matches<T> } -> std::convertible_to<bool>;
  typename C::Type;
};

template <typename T, MatchCase<T>... Cases>
struct FirstMatch {
 private:
  static constexpr std::array Hits{Cases::template Matches<T>...};
  static constexpr auto NumOfCases = sizeof...(Cases);

  [[nodiscard]] static consteval std::size_t FindFirstHit() noexcept {
    auto hit = std::ranges::find(Hits, true);
    auto idx = static_cast<std::size_t>(hit - Hits.begin());

    return idx;
  }

  static constexpr auto FirstHit = FindFirstHit();
  static_assert(FirstHit < NumOfCases, "typus::Match: no case matched and no Default provided");

 public:
  using Type = Cases...[FirstHit] ::Type;
};

template <typename... Cases>
  requires(sizeof...(Cases) > 0)
struct Match final : tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = base::Thunk<typename FirstMatch<Ts, Cases...>::Type...>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

template <typename... Cases>
inline constexpr auto Match = detail::Match<Cases...>{};

}  // namespace typus
