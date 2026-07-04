#include <typus/typus.hpp>

#include <gtest/gtest.h>

TEST(combine, sort) {
  static constexpr auto Tester = typus::From<char, double, int> | typus::SortBySize;

  using Get0 = typus::Get<Tester | typus::At<0>>;
  using Get1 = typus::Get<Tester | typus::At<1>>;
  using Get2 = typus::Get<Tester | typus::At<2>>;

  static_assert(std::same_as<Get0, char>);
  static_assert(std::same_as<Get1, int>);
  static_assert(std::same_as<Get2, double>);
}
