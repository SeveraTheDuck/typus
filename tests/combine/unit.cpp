#include <gtest/gtest.h>

#include <typus/typus.hpp>

#include <concepts>
#include <cstddef>
#include <tuple>
#include <type_traits>

using namespace typus;

template <typename T>
struct Explode {
  using type = base::Thunk<T, T*>;
};

TEST(combine, append) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int>
      | Append<float, double>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, at) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      ;
  // clang-format on

  static_assert(std::same_as<Get<Pipeline | At<0>>, int>);
  static_assert(std::same_as<Get<Pipeline | At<1>>, float>);
  static_assert(std::same_as<Get<Pipeline | At<2>>, double>);
}

TEST(combine, back) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      ;
  // clang-format on

  static_assert(std::same_as<Get<Pipeline | Back>, double>);
  static_assert(std::same_as<Get<From<char> | Back>, char>);
}

TEST(combine, bind) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      | Bind<From<double, char>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk
        < base::Pair<int, double>
        , base::Pair<int, char>
        , base::Pair<float, double>
        , base::Pair<float, char>
        >
      >
    >
  );

  static constexpr auto EmptyPipeline =
      Empty
      | Bind<From<double, char>>
      ;

  static_assert(std::same_as
    < Materialize<EmptyPipeline>
    , base::TypeTuple<base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, chunk) {
  // clang-format off
  static constexpr auto Pipeline =
      From<char, int, float, double, void>
      | Chunk<2>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk
        < base::Thunk<char, int>
        , base::Thunk<float, double>
        , base::Thunk<void>
        >
      >
    >
  );

  static constexpr auto ExactPipeline =
      From<int, float>
      | Chunk<2>
      ;

  static_assert(std::same_as
    < Materialize<ExactPipeline>
    , base::TypeTuple<base::Thunk<base::Thunk<int, float>>>
    >
  );

  static constexpr auto EmptyPipeline =
      Empty
      | Chunk<2>
      ;

  static_assert(std::same_as
    < Materialize<EmptyPipeline>
    , base::TypeTuple<base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, concat) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int>
      | Concat<From<float, double>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, drop) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      | Drop<1>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<float, double>>
    >
  );

  static constexpr auto OverdrawnPipeline =
      From<int, float, double>
      | Drop<5>
      ;

  static_assert(std::same_as
    < Materialize<OverdrawnPipeline>
    , base::TypeTuple<base::Empty>
    >
  );
  // clang-format on
}

TEST(combine, enumerate) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      | Enumerate
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk
        < base::Pair<int, std::integral_constant<std::size_t, 0>>
        , base::Pair<float, std::integral_constant<std::size_t, 1>>
        >
      >
    >
  );
  // clang-format on
}

TEST(combine, filter) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, int, double>
      | Filter<Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, int>>
    >
  );

  static constexpr auto EmptyPipeline =
      From<float, double>
      | Filter<Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < Materialize<EmptyPipeline>
    , base::TypeTuple<base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, find) {
  // clang-format off
  static constexpr auto Pipeline =
      From<float, int, double>
      | Find<Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int>>
    >
  );

  static constexpr auto MissingPipeline =
      From<float, double>
      | Find<Is<std::is_integral>>
      ;

  static_assert(std::same_as
    < Materialize<MissingPipeline>
    , base::TypeTuple<base::Thunk<>>
    >
  );
  // clang-format on
}

TEST(combine, flatten) {
  // clang-format off
  static constexpr auto Pipeline =
      From
        < base::Thunk<int, float>
        , base::Thunk<double>
        , base::Thunk<>
        >
      | Flatten
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, float, double>>
    >
  );

  static constexpr auto AllEmpty =
      From<base::Empty, base::Empty>
      | Flatten
      ;

  static_assert(std::same_as
    < Materialize<AllEmpty>
    , base::TypeTuple<base::Empty>
    >
  );
  // clang-format on
}

TEST(combine, fn) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int>
      | Fn<std::add_const>
      ;

  static_assert(std::same_as<Get<Pipeline>, const int>);
  // clang-format on
}

TEST(combine, force) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      | At<1>
      ;

  static_assert(std::same_as
    < detail::Force::Apply<detail::TypeOf<Pipeline>>
    , base::Singleton<float>
    >
  );
  // clang-format on
}

TEST(combine, front) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      | Front
      ;

  static_assert(std::same_as<Get<Pipeline>, int>);
  // clang-format on
}

TEST(combine, map) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      | Map<Fn<std::add_pointer>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int*, float*>>
    >
  );
  // clang-format on
}

TEST(combine, flatmap) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      | FlatMap<Fn<Explode>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, int*, float, float*>>
    >
  );
  // clang-format on
}

TEST(combine, match) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      | Map<Match
          < Case<Is<std::is_integral>, long>
          , Default<char>
          >
        >
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<long, char, char>>
    >
  );

  // First case wins over later ones
  static constexpr auto FirstWins =
      From<unsigned>
      | Map<Match
          < Case<Is<std::is_integral>, long>
          , Case<Is<std::is_unsigned>, short>
          , Default<char>
          >
        >
      ;
  static_assert(std::same_as
    < Materialize<FirstWins>
    , base::TypeTuple<base::Singleton<long>>  // integral matched first, not unsigned
    >
  );
  // clang-format on
}

TEST(combine, pair_accessors) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      ;

  static_assert(std::same_as<Get<Pipeline | First>, int>);
  static_assert(std::same_as<Get<Pipeline | Second>, float>);
  // clang-format on
}

TEST(combine, prepend) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float>
      | Prepend<char, double>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<char, double, int, float>>
    >
  );
  // clang-format on
}

TEST(combine, reverse) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      | Reverse
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<double, float, int>>
    >
  );
  // clang-format on
}

TEST(combine, sort) {
  // clang-format off
  static constexpr auto Pipeline =
      From<double, char, int>
      | SortBySize
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<char, int, double>>
    >
  );

  static constexpr auto DescPipeline =
      From<char, double, int>
      | SortBySizeDec
      ;

  static_assert(std::same_as
    < Materialize<DescPipeline>
    , base::TypeTuple<base::Thunk<double, int, char>>
    >
  );
  // clang-format on
}

TEST(combine, split_by) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, void, double, void, char>
      | SplitBy<Is<std::is_void>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk
        < base::Thunk<int, float>
        , base::Thunk<double>
        , base::Thunk<char>
        >
      >
    >
  );

  // Leading / trailing / consecutive delimiters produce empty chunks
  static constexpr auto Edges =
      From<void, int, void, void, char, void>
      | SplitBy<Is<std::is_void>>
      ;

  static_assert(std::same_as
    < Materialize<Edges>
    , base::TypeTuple<base::Thunk
        < base::Empty
        , base::Thunk<int>
        , base::Empty
        , base::Thunk<char>
        , base::Empty
        >
      >
    >
  );
  // clang-format on
}

TEST(combine, logic) {
  // clang-format off
  static constexpr auto NotPipeline =
      From<int>
      | Map<Is<std::is_integral>>
      | Not
      ;

  static_assert(std::same_as
    < Materialize<NotPipeline>
    , base::TypeTuple<base::Thunk<std::bool_constant<false>>>
    >
  );

  static constexpr auto AndPipeline =
      From<int>
      | Map<And<Is<std::is_integral>, Is<std::is_unsigned>>>
      ;

  static_assert(std::same_as
    < Materialize<AndPipeline>
    , base::TypeTuple<base::Thunk<std::bool_constant<false>>>
    >
  );

  static constexpr auto OrPipeline =
      From<void>
      | Map<Or<Is<std::is_integral>, Is<std::is_void>>>
      ;

  static_assert(std::same_as
    < Materialize<OrPipeline>
    , base::TypeTuple<base::Thunk<std::bool_constant<true>>>
    >
  );
  // clang-format on
}

TEST(combine, take) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, double>
      | Take<2>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, float>>
    >
  );

  static_assert(std::same_as
    < Materialize<From<int, float> | Take<5>>
    , base::TypeTuple<base::Thunk<int, float>>
    >
  );

  static_assert(std::same_as
    < Materialize<From<int, float> | Take<0>>
    , base::TypeTuple<base::Empty>
    >
  );
  // clang-format on
}

TEST(combine, unique) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, float, int, double, float>
      | Unique
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, float, double>>
    >
  );

  static_assert(std::same_as
    < Materialize<From<int, int, int> | Unique>
    , base::TypeTuple<base::Singleton<int>>
    >
  );
  // clang-format on
}

TEST(combine, unpack) {
  // clang-format off
  static constexpr auto Pipeline =
      From<std::tuple<int, float>, std::tuple<double>>
      | Unpack
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk<int, float, double>>
    >
  );
  // clang-format on
}

TEST(combine, zip) {
  // clang-format off
  static constexpr auto Pipeline =
      From<int, double>
      | Zip<From<float, char>>
      ;

  static_assert(std::same_as
    < Materialize<Pipeline>
    , base::TypeTuple<base::Thunk
        < base::Pair<int, float>
        , base::Pair<double, char>
        >
      >
    >
  );

  // Shorter side wins
  static constexpr auto Uneven =
      From<int, double, long>
      | Zip<From<float, char>>
      ;

  static_assert(std::same_as
    < Materialize<Uneven>
    , base::TypeTuple<base::Thunk
        < base::Pair<int, float>
        , base::Pair<double, char>
        >
      >
    >
  );

  // Empty side -> empty result
  static constexpr auto WithEmpty =
      From<int, double>
      | Zip<Empty>
      ;

  static_assert(std::same_as
    < Materialize<WithEmpty>
    , base::TypeTuple<base::Empty>
    >
  );

  // Zip | Flatten interleaves
  static constexpr auto Interleaved =
      From<int, double>
      | Zip<From<float, char>>
      | Flatten
      ;

  static_assert(std::same_as
    < Materialize<Interleaved>
    , base::TypeTuple<base::Thunk<int, float, double, char>>
    >
  );
  // clang-format on
}
