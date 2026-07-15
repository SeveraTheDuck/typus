#include <gtest/gtest.h>

#include <typus/typus.hpp>

#include <concepts>
#include <cstddef>
#include <tuple>
#include <type_traits>

template <typename T>
struct Explode {
  using type = typus::base::Thunk<T, T*>;
};

TEST(combine, append) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int>
      | typus::Append<float, double>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, at) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      ;
  // clang-format on

  static_assert(std::same_as<typus::Get<Pipeline | typus::At<0>>, int>);
  static_assert(std::same_as<typus::Get<Pipeline | typus::At<1>>, float>);
  static_assert(std::same_as<typus::Get<Pipeline | typus::At<2>>, double>);
}

TEST(combine, back) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      ;
  // clang-format on

  static_assert(std::same_as<typus::Get<Pipeline | typus::Back>, double>);
  static_assert(std::same_as<typus::Get<typus::From<char> | typus::Back>, char>);
}

TEST(combine, bind) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      | typus::Bind<typus::From<double, char>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk
        < typus::base::Pair<int, double>
        , typus::base::Pair<int, char>
        , typus::base::Pair<float, double>
        , typus::base::Pair<float, char>
        >
      >
    >
  );

  static constexpr auto EmptyPipeline =
      typus::Empty
      | typus::Bind<typus::From<double, char>>
      ;

  static_assert(std::same_as
    < typus::Materialize<EmptyPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, chunk) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<char, int, float, double, void>
      | typus::Chunk<2>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk
        < typus::base::Thunk<char, int>
        , typus::base::Thunk<float, double>
        , typus::base::Thunk<void>
        >
      >
    >
  );

  static constexpr auto ExactPipeline =
      typus::From<int, float>
      | typus::Chunk<2>
      ;

  static_assert(std::same_as
    < typus::Materialize<ExactPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<typus::base::Thunk<int, float>>>
    >
  );

  static constexpr auto EmptyPipeline =
      typus::Empty
      | typus::Chunk<2>
      ;

  static_assert(std::same_as
    < typus::Materialize<EmptyPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, concat) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int>
      | typus::Concat<typus::From<float, double>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, drop) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      | typus::Drop<1>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<float, double>>
    >
  );

  static constexpr auto OverdrawnPipeline =
      typus::From<int, float, double>
      | typus::Drop<5>
      ;

  static_assert(std::same_as
    < typus::Materialize<OverdrawnPipeline>
    , typus::base::TypeTuple<typus::base::Empty>
    >
  );
  // clang-format on
}

TEST(combine, enumerate) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      | typus::Enumerate
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk
        < typus::base::Pair<int, std::integral_constant<std::size_t, 0>>
        , typus::base::Pair<float, std::integral_constant<std::size_t, 1>>
        >
      >
    >
  );
  // clang-format on
}

TEST(combine, filter) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, int, double>
      | typus::Filter<typus::Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, int>>
    >
  );

  static constexpr auto EmptyPipeline =
      typus::From<float, double>
      | typus::Filter<typus::Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < typus::Materialize<EmptyPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, find) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<float, int, double>
      | typus::Find<typus::Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int>>
    >
  );

  static constexpr auto MissingPipeline =
      typus::From<float, double>
      | typus::Find<typus::Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < typus::Materialize<MissingPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, flatten) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From
        < typus::base::Thunk<int, float>
        , typus::base::Thunk<double>
        , typus::base::Thunk<>
        >
      | typus::Flatten
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, fn) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int>
      | typus::Fn<std::add_const>
      ;

  static_assert(std::same_as<typus::Get<Pipeline>, const int>);
  // clang-format on
}

TEST(combine, force) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      | typus::At<1>
      ;

  static_assert(std::same_as
    < typus::detail::Force::Apply<typus::detail::TypeOf<Pipeline>>
    , typus::base::Singleton<float>
    >
  );
  // clang-format on
}

TEST(combine, front) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      | typus::Front
      ;

  static_assert(std::same_as<typus::Get<Pipeline>, int>);
  // clang-format on
}

TEST(combine, map) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      | typus::Map<typus::Fn<std::add_pointer>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int*, float*>>
    >
  );
  // clang-format on
}

TEST(combine, flatmap) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      | typus::FlatMap<typus::Fn<Explode>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, int*, float, float*>>
    >
  );
  // clang-format on
}

TEST(combine, match) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      | typus::Map<typus::Match
          < typus::Case<typus::Is<std::is_integral>, long>
          , typus::Default<char>
          >
        >
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<long, char, char>>
    >
  );
  // clang-format on
}

TEST(combine, pair_accessors) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      ;

  static_assert(std::same_as<typus::Get<Pipeline | typus::First>, int>);
  static_assert(std::same_as<typus::Get<Pipeline | typus::Second>, float>);
  // clang-format on
}

TEST(combine, prepend) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float>
      | typus::Prepend<char, double>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<char, double, int, float>>
    >
  );
  // clang-format on
}

TEST(combine, reverse) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      | typus::Reverse
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<double, float, int>>
    >
  );
  // clang-format on
}

TEST(combine, sort) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<double, char, int>
      | typus::SortBySize
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<char, int, double>>
    >
  );

  static constexpr auto DescPipeline =
      typus::From<char, double, int>
      | typus::SortBySizeDec
      ;

  static_assert(std::same_as
    < typus::Materialize<DescPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<double, int, char>>
    >
  );
  // clang-format on
}

TEST(combine, split_by) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, void, double, void, char>
      | typus::SplitBy<typus::Is<std::is_void>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk
        < typus::base::Thunk<int, float>
        , typus::base::Thunk<double>
        , typus::base::Thunk<char>
        >
      >
    >
  );
  // clang-format on
}

TEST(combine, logic) {
  // clang-format off
  static constexpr auto NotPipeline =
      typus::From<int>
      | typus::Map<typus::Is<std::is_integral>>
      | typus::Not
      ;

  static_assert(std::same_as
    < typus::Materialize<NotPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<std::bool_constant<false>>>
    >
  );

  static constexpr auto AndPipeline =
      typus::From<int>
      | typus::Map<typus::And<typus::Is<std::is_integral>, typus::Is<std::is_unsigned>>>
      ;

  static_assert(std::same_as
    < typus::Materialize<AndPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<std::bool_constant<false>>>
    >
  );

  static constexpr auto OrPipeline =
      typus::From<void>
      | typus::Map<typus::Or<typus::Is<std::is_integral>, typus::Is<std::is_void>>>
      ;

  static_assert(std::same_as
    < typus::Materialize<OrPipeline>
    , typus::base::TypeTuple<typus::base::Thunk<std::bool_constant<true>>>
    >
  );
  // clang-format on
}

TEST(combine, take) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, double>
      | typus::Take<2>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, float>>
    >
  );
  // clang-format on
}

TEST(combine, unique) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, float, int, double, float>
      | typus::Unique
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, unpack) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<std::tuple<int, float>, std::tuple<double>>
      | typus::Unpack
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, zip) {
  // clang-format off
  static constexpr auto Pipeline =
      typus::From<int, double>
      | typus::Zip<typus::From<float, char>>
      ;

  static_assert(std::same_as
    < typus::Materialize<Pipeline>
    , typus::base::TypeTuple<typus::base::Thunk
        < typus::base::Pair<int, float>
        , typus::base::Pair<double, char>
        >
      >
    >
  );
  // clang-format on
}
