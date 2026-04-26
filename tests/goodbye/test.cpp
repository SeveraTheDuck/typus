#include <project_name/goodbye/goodbye.hpp>

#include <gtest/gtest.h>

TEST(GoodbyeTest, PrintDoesNotCrash) {
  EXPECT_NO_THROW(goodbye::Print());
}
