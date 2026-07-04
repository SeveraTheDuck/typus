#include <typus/typus.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace {

TEST(make, from) {
  static_assert(
      std::same_as<decltype(typus::From<int, double>), const typus::base::Thunk<int, double>>);
}

TEST(make, empty) {
  static_assert(std::same_as<decltype(typus::Empty), const typus::base::Thunk<>>);
}

}  // namespace
