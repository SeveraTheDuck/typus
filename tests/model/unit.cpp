#include <typus/typus.hpp>

#include <gtest/gtest.h>

TEST(model, thunk) {
  using Tester = decltype(typus::From<int, double>);
  static_assert(typus::model::Thunk<Tester>);
}
