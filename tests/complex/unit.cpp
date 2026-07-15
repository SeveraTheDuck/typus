#include <typus/typus.hpp>

#include <concepts>
#include <cstdint>
#include <type_traits>

#include <gtest/gtest.h>

namespace {

struct SmallBox {};
struct BigBox {};
struct PtrBox {};

template <typename T>
struct Small : std::bool_constant<(sizeof(T) <= 4)> {};

}  // namespace

TEST(complex, integration) {
  static constexpr auto CleanArithmetic =
      typus::Map<typus::Fn<std::remove_cvref>> | typus::Filter<typus::Is<std::is_arithmetic>>;

  // clang-format off
  static constexpr auto Pipeline =
    typus::From
      < const std::int64_t&
      , void*
      , std::uint8_t
      , double&&
      , const char*
      , float
      >
    | CleanArithmetic
    | typus::SortBySize
    | typus::Map
      < typus::Match
        < typus::Case<typus::Is<Small>, SmallBox>
        , typus::Case<typus::Is<std::is_pointer>, PtrBox>
        , typus::Default<BigBox>
        >
      >
    ;
  // clang-format on

  // TypeTuple<SmallBox, SmallBox, BigBox, BoxBox>
  using Result = typus::Materialize<Pipeline>;

  static_assert(Result::Size == 4);

  static_assert(std::same_as<Result::At<0>, SmallBox>);
  static_assert(std::same_as<Result::At<1>, SmallBox>);
  static_assert(std::same_as<Result::At<2>, BigBox>);
  static_assert(std::same_as<Result::At<3>, BigBox>);

  // clang-format off
  static constexpr auto CountBig =
      CleanArithmetic
      | typus::Filter
        < typus::Is<Small> | typus::Not >
      | typus::Size;

  static_assert(
    (
      typus::From
        < const std::int64_t&, void*
        , std::uint8_t
        , double&&
        , const char*
        , float
        >
      | CountBig
    ) == 2);
  // clang-format on
}
