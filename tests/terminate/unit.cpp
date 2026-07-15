#include <gtest/gtest.h>

#include <typus/typus.hpp>

#include <type_traits>

using namespace typus;

// Helper trait to use in predicate tests
template <typename T>
struct IsInt : std::is_same<T, int> {};

// Helper types containing constant values for Sum and Product test
using IntMinus5 = std::integral_constant<int, -5>;
using IntTen = std::integral_constant<int, 10>;

TEST(terminate, all_any_none) {
  constexpr auto IntPred = Is<IsInt>;

  // All
  static_assert((From<int, int> | All<IntPred>) == true);
  static_assert((From<int, float> | All<IntPred>) == false);
  static_assert((Empty | All<IntPred>) == true);

  // Any
  static_assert((From<float, int> | Any<IntPred>) == true);
  static_assert((From<float, double> | Any<IntPred>) == false);
  static_assert((Empty | Any<IntPred>) == false);

  // None
  static_assert((From<float, double> | None<IntPred>) == true);
  static_assert((From<float, int> | None<IntPred>) == false);
  static_assert((Empty | None<IntPred>) == true);
}

TEST(terminate, search) {
  using Source = base::Thunk<float, double, int>;

  // Contains
  static_assert((Source{} | Contains<double>) == true);
  static_assert((Source{} | Contains<char>) == false);

  // IndexOf
  static_assert((Source{} | IndexOf<double>) == 1);
  static_assert((Source{} | IndexOf<int>) == 2);
  static_assert((Source{} | IndexOf<char>) == 3); // Out of bounds equals Size
}

TEST(terminate, size) {
  static_assert((From<int, float, double> | Size) == 3);
  static_assert((Empty | Size) == 0);
}

TEST(terminate, sum) {
  // Regular case deduces `int` properly
  static_assert((From<IntMinus5, IntTen> | Sum) == 5);
  static_assert(std::same_as<detail::TypeOf<From<IntMinus5, IntTen> | Sum>, int>);

  // Empty case defaults to `size_t`
  static_assert((Empty | Sum) == 0);
  static_assert(std::same_as<detail::TypeOf<Empty | Sum>, std::size_t>);
}

TEST(terminate, product) {
  // Regular case deduces `int` properly
  static_assert((From<IntMinus5, IntTen> | Product) == -50);
  static_assert(std::same_as<detail::TypeOf<From<IntMinus5, IntTen> | Product>, int>);

  // Empty case defaults to `size_t`
  static_assert((Empty | Product) == 1);
  static_assert(std::same_as<detail::TypeOf<Empty | Product>, std::size_t>);
}

TEST(terminate, get) {
  static_assert(std::same_as<Get<From<int, float, double> | At<1>>, float>);
}

TEST(terminate, materialize) {
  using Frozen = Materialize<From<int, float> | Reverse>;

  static_assert(Frozen::Size == 2);
  static_assert(std::same_as<Frozen::At<0>, float>);
  static_assert(std::same_as<Frozen::At<1>, int>);
}
