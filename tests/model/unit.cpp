#include <gtest/gtest.h>

#include <typus/typus.hpp>

using namespace typus;

// Dummy types for testing concepts
struct DummyCombinator : tag::Combinator {};
struct DummyTerminator : tag::ValueTerminator {};
struct NotAnOperation {};

TEST(model, thunk) {
  static_assert(model::Thunk<base::Thunk<int, float>>);
  static_assert(model::Thunk<base::Thunk<>>);

  // Checking cvref modifiers
  static_assert(model::Thunk<const base::Thunk<int>&>);
  static_assert(model::Thunk<volatile base::Thunk<double>&&>);

  static_assert(!model::Thunk<int>);
  static_assert(!model::Thunk<DummyCombinator>);
}

TEST(model, combinator) {
  static_assert(model::Combinator<DummyCombinator>);

  // Checking cvref modifiers
  static_assert(model::Combinator<const DummyCombinator&>);
  static_assert(model::Combinator<DummyCombinator&&>);

  static_assert(!model::Combinator<base::Thunk<int>>);
  static_assert(!model::Combinator<DummyTerminator>);
  static_assert(!model::Combinator<NotAnOperation>);
}

TEST(model, terminator) {
  static_assert(model::Terminator<DummyTerminator>);

  // Checking cvref modifiers
  static_assert(model::Terminator<const DummyTerminator&>);

  static_assert(!model::Terminator<base::Thunk<int>>);
  static_assert(!model::Terminator<DummyCombinator>);
  static_assert(!model::Terminator<NotAnOperation>);
}

TEST(model, operation) {
  static_assert(model::Operation<DummyCombinator>);
  static_assert(model::Operation<DummyTerminator>);

  // Checking cvref modifiers
  static_assert(model::Operation<const DummyCombinator&>);
  static_assert(model::Operation<volatile DummyTerminator&&>);

  static_assert(!model::Operation<base::Thunk<int>>);
  static_assert(!model::Operation<NotAnOperation>);
}

TEST(model, anchored) {
  static_assert(model::Anchored<base::Thunk<int>>);
  static_assert(model::Anchored<base::Thunk<>>);

  // Checking cvref modifiers
  static_assert(model::Anchored<const base::Thunk<int>&>);

  static_assert(!model::Anchored<DummyCombinator>);
  static_assert(!model::Anchored<DummyTerminator>);
  static_assert(!model::Anchored<NotAnOperation>);
}

TEST(model, pipe_expr) {
  static_assert(model::PipeExpr<base::Thunk<int>>);
  static_assert(model::PipeExpr<DummyCombinator>);

  // Checking cvref modifiers
  static_assert(model::PipeExpr<const base::Thunk<int>&>);
  static_assert(model::PipeExpr<DummyCombinator&&>);

  // Terminators end the pipe, they can't be on LHS
  static_assert(!model::PipeExpr<DummyTerminator>);
  static_assert(!model::PipeExpr<NotAnOperation>);
}
