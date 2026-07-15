#include <gtest/gtest.h>

#include <typus/typus.hpp>

#include <concepts>
#include <cstddef>
#include <type_traits>

using namespace typus;

TEST(make, empty) {
  // clang-format off
  static_assert(std::same_as
    <
      detail::TypeOf<Empty>,
      base::Thunk<>
    >
  );
  // clang-format on
}

TEST(make, from) {
  // clang-format off
  static_assert(std::same_as
    <
      detail::TypeOf<From<int, double>>,
      base::Thunk<int, double>
    >
  );
  // clang-format on
}

TEST(make, iota) {
  // clang-format off
  static_assert(std::same_as
    <
      detail::TypeOf<Iota<2>>,
      base::Thunk
        <
          std::integral_constant<std::size_t, 0>,
          std::integral_constant<std::size_t, 1>
        >
    >
  );

  static_assert(std::same_as
    <
      detail::TypeOf<Iota<0>>,
      base::Empty
    >
  );
  // clang-format on
}

TEST(make, repeat) {
  // clang-format off
  static_assert(std::same_as
    <
      detail::TypeOf<Repeat<int, 3>>,
      base::Thunk
        <
          int,
          int,
          int
        >
    >
  );

  static_assert(std::same_as
    <
      detail::TypeOf<Repeat<double, 0>>,
      base::Empty
    >
  );
  // clang-format on
}

TEST(make, singleton) {
  // clang-format off
  static_assert(std::same_as
    <
      detail::TypeOf<Singleton<int>>,
      base::Singleton<int>
    >
  );

  static_assert(std::same_as
    <
      detail::TypeOf<Singleton<int>>,
      base::Thunk<int>
    >
  );
  // clang-format on
}
