/**
 * @file match.hpp
 * @author SeveraTheDuck
 * @brief Pattern matching combinator for type transformation.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/holds.hpp>

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>

namespace typus {

/**
 * @brief A branch in a Match expression.
 */
template <model::Combinator auto Predicate, typename R>
struct Case final {
  template <typename T>
  static constexpr bool Matches = detail::Holds<Predicate, T>;

  using Type = R;
};

/**
 * @brief The fallback branch in a Match expression.
 */
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

  template <typename T>
  struct Impl<base::Singleton<T>> {
    using Type = base::Thunk<typename FirstMatch<T, Cases...>::Type>;
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Performs pattern matching to transform a single type.
 *
 * Evaluates the provided type against a series of `Case` predicates in order.
 * Replaces the input type with the `Type` of the first matching `Case`.
 * If no `Case` matches and a `Default` branch is provided, it yields the
 * `Default` type. If no branch matches, a compilation error is triggered.
 *
 * @tparam Cases The `Case` or `Default` branches.
 *
 * @par Example
 * @code
 * constexpr auto pipeline = typus::Match
 * <
 *   typus::Case<typus::Is<std::is_integral>, long>,
 *   typus::Default<char>
 * >;
 *
 * using T1 = typus::Get<typus::From<int> | pipeline>;    // long
 * using T2 = typus::Get<typus::From<float> | pipeline>;  // char
 * @endcode
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, float, double> | typus::Map
 * <
 *   typus::Match
 *   <
 *     typus::Case<typus::Is<std::is_integral>, long>,
 *     typus::Default<char>
 *   >
 * >;
 * // Result: Thunk<long, char, char>
 * @endcode
 */
template <typename... Cases>
inline constexpr auto Match = detail::Match<Cases...>{};

}  // namespace typus
