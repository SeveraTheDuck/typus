/**
 * @file normalize_naming.hpp
 * @author SeveraTheDuck
 * @brief Utilities for compatibility between STL snake_case and Typus CamelCase traits.
 */
#pragma once

#include <concepts>
#include <type_traits>

// Compatibility with std snake_case and user's CamelCase styles in predicates and meta functions

namespace typus::detail {

// Normalize P::value and P::Value variants

template <typename P>
concept HasSnakeCaseValue = requires { P::value; };

template <typename P>
concept HasCamelCaseValue = requires { P::Value; };

template <typename P>
concept HasDivergingValueDefinitions =
    HasSnakeCaseValue<P> and HasCamelCaseValue<P> and (P::value != P::Value);

template <typename P>
struct NormalizeValue {
 private:
  static_assert(
      HasSnakeCaseValue<P> or HasCamelCaseValue<P>,
      "typus: predicate exposes neither ::value nor ::Value");

  static_assert(
      not HasDivergingValueDefinitions<P>,
      "typus: predicate exposes both ::value and ::Value with diverging results");

  [[nodiscard]] static consteval auto Pick() noexcept
    requires HasSnakeCaseValue<P>
  {
    return P::value;
  }

  [[nodiscard]] static consteval auto Pick() noexcept
    requires(HasCamelCaseValue<P> and not HasSnakeCaseValue<P>)
  {
    return P::Value;
  }

 public:
  static constexpr auto Value = Pick();
};

// Normalize F::type and F::Type variants

template <typename F>
concept HasSnakeCaseType = requires { typename F::type; };

template <typename F>
concept HasCamelCaseType = requires { typename F::Type; };

template <typename F>
concept HasDivergingTypeDefinitions = HasSnakeCaseType<F> and HasCamelCaseType<F> and
                                      (not std::same_as<typename F::type, typename F::Type>);

template <typename F>
struct NormalizeMetaFnType {
 private:
  static_assert(
      HasSnakeCaseType<F> or HasCamelCaseType<F>,
      "typus: meta function exposes neither ::type nor ::Type");

  static_assert(
      not HasDivergingTypeDefinitions<F>,
      "typus: meta function exposes both ::type and ::Type with diverging results");

  [[nodiscard]] static consteval auto Pick() noexcept
    requires HasSnakeCaseType<F>
  {
    return std::type_identity<typename F::type>{};
  }

  [[nodiscard]] static consteval auto Pick() noexcept
    requires(HasCamelCaseType<F> and not HasSnakeCaseType<F>)
  {
    return std::type_identity<typename F::Type>{};
  }

 public:
  using Type = decltype(Pick())::type;
};

}  // namespace typus::detail
