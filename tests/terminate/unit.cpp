#include <typus/typus.hpp>

#include <gtest/gtest.h>

#include <concepts>

TEST(terminate, get) {
  static constexpr auto list = typus::From<int, bool, int, double>;

  using Get0 = typus::Get<list | typus::Front>;
  using Get1 = typus::Get<list | typus::At<1>>;
  using Get3 = typus::Get<list | typus::Back>;

  static_assert(std::same_as<Get0, int>);
  static_assert(std::same_as<Get1, bool>);
  static_assert(std::same_as<Get3, double>);
}

TEST(terminate, size) {
  static constexpr auto Tester1 = typus::From<int, double>;
  static constexpr auto Tester2 = typus::Empty;

  static_assert((Tester1 | typus::Size) == 2);
  static_assert((Tester2 | typus::Size) == 0);
}
