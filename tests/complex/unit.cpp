#include <typus/typus.hpp>

#include <concepts>
#include <cstdint>
#include <type_traits>

#include <gtest/gtest.h>

using namespace typus;

namespace {

struct SmallBox {};
struct BigBox {};
struct PtrBox {};

template <typename T>
struct Small : std::bool_constant<(sizeof(T) <= 4)> {};

template <typename T>
struct Dup {
  using type = base::Thunk<T, T>;
};

template <typename T>
struct Pure {
  using type = base::Singleton<T>;
};

template <typename T>
struct Ptr {
  using type = base::Singleton<T*>;
};

// clang-format off
template <typename T>
struct BindFG {
  using type = detail::Finalize
  < typename Dup<T>::type{}
  | FlatMap<Fn<Ptr>>
  >;
};
// clang-format on

}  // namespace

TEST(complex, monad_laws) {
  // clang-format off

  // --- Left identity:  pure(a) >>= f  ==  f(a) ---
  // From<int> | FlatMap<Pure> | FlatMap<Dup>  ==  From<int> | FlatMap<Dup>
  static constexpr auto LeftLhs =
      From<int>
      | FlatMap<Fn<Pure>>
      | FlatMap<Fn<Dup>>
      ;
  static constexpr auto LeftRhs =
      From<int>
      | FlatMap<Fn<Dup>>
      ;
  static_assert(std::same_as
    < Materialize<LeftLhs>
    , Materialize<LeftRhs>
    >
  );

  // --- Right identity:  m >>= pure  ==  m ---
  // From<int, float> | FlatMap<Pure>  ==  From<int, float>
  static constexpr auto RightLhs =
      From<int, float>
      | FlatMap<Fn<Pure>>
      ;
  static constexpr auto RightRhs =
      From<int, float>
      ;
  static_assert(std::same_as
    < Materialize<RightLhs>
    , Materialize<RightRhs>
    >
  );

  // --- Associativity:  (m >>= f) >>= g  ==  m >>= (\x -> f(x) >>= g) ---
  static constexpr auto AssocLhs =
      From<int, float>
      | FlatMap<Fn<Dup>>
      | FlatMap<Fn<Ptr>>
      ;

  static constexpr auto AssocRhs =
      From<int, float>
      | FlatMap<Fn<BindFG>>
      ;

  static_assert(std::same_as
    < Materialize<AssocLhs>
    , Materialize<AssocRhs>
    >
  );

  // Left identity holds on empty input too
  static_assert(std::same_as
    < Materialize<Empty
      | FlatMap<Fn<Pure>>
      | FlatMap<Fn<Dup>>>
    , Materialize<Empty
      | FlatMap<Fn<Dup>>>
    >
  );

  // clang-format on
}

TEST(complex, integration) {
  static constexpr auto CleanArithmetic =
      Map<Fn<std::remove_cvref>> | Filter<Is<std::is_arithmetic>>;

  // clang-format off
  static constexpr auto Pipeline =
    From
      < const std::int64_t&
      , void*
      , std::uint8_t
      , double&&
      , const char*
      , float
      >
    | CleanArithmetic
    | SortBySize
    | Map
      < Match
        < Case<Is<Small>, SmallBox>
        , Case<Is<std::is_pointer>, PtrBox>
        , Default<BigBox>
        >
      >
    ;
  // clang-format on

  // TypeTuple<SmallBox, SmallBox, BigBox, BigBox>
  using Result = Materialize<Pipeline>;

  static_assert(Result::Size == 4);

  static_assert(std::same_as<Result::At<0>, SmallBox>);
  static_assert(std::same_as<Result::At<1>, SmallBox>);
  static_assert(std::same_as<Result::At<2>, BigBox>);
  static_assert(std::same_as<Result::At<3>, BigBox>);

  // clang-format off
  static constexpr auto CountBig =
      CleanArithmetic
      | Filter
        < Is<Small> | Not >
      | Size;

  static constexpr auto pipeline =
    From
      < const std::int64_t&, void*
      , std::uint8_t
      , double&&
      , const char*
      , float
      >;

  static_assert((pipeline | CountBig) == 2);  // std::int64_t, double

  // clang-format on
}
