#include <typus/greeting/greeting.hpp>

#include <gtest/gtest.h>

namespace {

TEST(greeting, returns_hello) {
  EXPECT_EQ(typus::Greeting("world"), "Hello, world!");
}

}  // namespace
