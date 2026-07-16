# typus

C++26 header-only type metaprogramming library. Functional, pipeline-driven API, lazy evaluation

<!-- === templates-base badges === -->
[![CI](https://github.com/SeveraTheDuck/typus/actions/workflows/ci.yaml/badge.svg)](https://github.com/SeveraTheDuck/typus/actions/workflows/ci.yaml)
[![Release](https://img.shields.io/github/v/release/SeveraTheDuck/typus)](https://github.com/SeveraTheDuck/typus/releases)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSES/MIT.txt)
[![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-yellow.svg)](https://conventionalcommits.org)
[![Renovate enabled](https://img.shields.io/badge/renovate-enabled-brightgreen.svg)](https://renovatebot.com)
<!-- === end templates-base badges === -->
<!-- === templates-lang-cpp badges === -->
[![CI C++](https://github.com/SeveraTheDuck/typus/actions/workflows/ci-cpp.yaml/badge.svg)](https://github.com/SeveraTheDuck/typus/actions/workflows/ci-cpp.yaml)
[![codecov](https://codecov.io/gh/SeveraTheDuck/typus/graph/badge.svg)](https://codecov.io/gh/SeveraTheDuck/typus)
[![C++26](https://img.shields.io/badge/C%2B%2B-26-blue.svg)](https://en.cppreference.com/w/cpp)
<!-- === end templates-lang-cpp badges === -->

## Table of contents

- [Overview](#overview)
- [Quick start](#quick-start)
- [Core model](#core-model)
  - [Thunk: the data carrier](#thunk-the-data-carrier)
  - [Lazy pipelines and evaluation points](#lazy-pipelines-and-evaluation-points)
  - [TypeTuple: the frozen result](#typetuple-the-frozen-result)
- [Tour of the library](#tour-of-the-library)
  - [Makers](#makers)
  - [Combinators](#combinators)
  - [Predicates](#predicates)
  - [Terminators](#terminators)
- [Showcase](#showcase)
  - [Match: type-level pattern matching](#match-type-level-pattern-matching)
  - [FlatMap: the list monad at type level](#flatmap-the-list-monad-at-type-level)
  - [TypeTuple::As: escaping into the real world](#typetupleas-a-door-into-your-application)
- [Extending typus](#extending-typus)
  - [Writing your own combinator](#writing-your-own-combinator)
  - [Writing your own terminator](#writing-your-own-terminator)
- [Development](#development)
- [Using this library](#using-this-library)
- [Contributing](#contributing)
- [License](#license)

## Overview

typus is a type-list manipulation library built around three ideas:

1. **Pipelines, not nested templates.** Transformations compose left to right
   with `operator|`, the way you read them.
2. **Lazy by construction.** A pipeline is an AST. Nothing is evaluated until
   the pipeline meets a *terminator* (`Get`, `Materialize`, `Size`, ...) or is
   explicitly evaluated with `Force`. Until then, you are just building a value
   you can name, store, and reuse.
3. **C++26 reflection under the hood.** Sorting, filtering, deduplication and
   others run as ordinary `consteval` algorithms over `std::meta::info`
   arrays. No O(n²) recursive template instantiations.

```cpp
#include <typus/typus.hpp>

using Result = typus::Materialize
<
  typus::From<const std::uint32_t&, void*, char, double&&>
  | typus::Map<typus::Fn<std::remove_cvref>>
  | typus::Filter<typus::Is<std::is_arithmetic>>
  | typus::SortBySize
>;
// Result is a TypeTuple over Thunk<char, std::uint32_t, double>
static_assert(std::same_as<Result::At<0>, char>);
```

## Quick start

```cpp
#include <typus/typus.hpp>

// A pipeline is a constexpr value. This line computes nothing:
static constexpr auto pipeline =
  typus::From<int, float, int, double>
  | typus::Unique
  | typus::Reverse;

// Evaluation happens here, at the terminator:
using Frozen = typus::Materialize<pipeline>;  // TypeTuple<Thunk<double, float, int>>

// Or extract a single type:
using First = typus::Get<pipeline | typus::Front>;  // double

// Or a plain value — eager operator| fires when data meets a terminator:
static_assert((pipeline | typus::Size) == 3);
```

## Core model

### Thunk: the data carrier

`typus::base::Thunk<Ts...>` is a phantom type holding a pack of types. It is
the normal form every combinator consumes and produces. Three aliases name the
common shapes:

| Alias                | Meaning                     |
|----------------------|-----------------------------|
| `base::Empty`        | `Thunk<>` — no types        |
| `base::Singleton<T>` | `Thunk<T>` — exactly one    |
| `base::Pair<A, B>`   | `Thunk<A, B>` — exactly two |

Value-level entry points live in the top namespace: `typus::From<Ts...>`,
`typus::Empty`, `typus::Singleton<T>`, `typus::Pair<A, B>`, plus the
generators `typus::Iota<N>` and `typus::Repeat<T, N>`.

### Lazy pipelines and evaluation points

`operator|` does not compute, but records. Each `|` builds one node of an AST
(`Chain`), and the whole pipeline is just a `constexpr` value describing *what
to do* — not the result of doing it:

```cpp
// A reusable entry. Zero computation happened here:
static constexpr auto CleanArithmetic =
    typus::Map
    < typus::Fn<std::remove_cvref>
    >
    | typus::Filter
    < typus::Is<std::is_arithmetic>
    >;

// Still nothing:
static constexpr auto pipeline = typus::From<const int&, void*> | CleanArithmetic;
```

Evaluation is triggered at exactly three kinds of places:

1. **`Get` / `Materialize`** — the terminal facades. They force the whole
   chain and hand you the result (a single type, or a `TypeTuple`).
2. **A value terminator meeting anchored data.** When a pipeline that carries
   data (`From<...> | ...`) is piped into `Size`, `Contains<T>`, `All<P>`,
   `Sum`, ... the eager `operator|` overload fires and returns a plain value.
3. **`Force`** — the explicit evaluation combinator, when you want to force a
   sub-pipeline in the middle of building a bigger one.

A pipeline without data (starting from a combinator, not a `Thunk`) is itself
a combinator: it can be stored, named, passed into `Filter`/`Map`/`Case`
slots, and composed further. Functions and data live in one algebra.

### TypeTuple: the frozen result

`Materialize` returns a `base::TypeTuple` — an immutable, inspectable
interface over the evaluated Thunk. Materializing is one-way by design: there
is no route from a TypeTuple back into a pipeline.

```cpp
using Frozen = typus::Materialize<pipeline>;

Frozen::Size            // element count
Frozen::At<N>           // type at index N
Frozen::As<std::tuple>  // rebind into any variadic template template parameter
```

`As<Target>` is the door out of typus and into your application — see the
[showcase](#typetupleas-a-door-into-your-application).

## Tour of the library

### Makers

| Maker          | Result                                                                     |
|----------------|----------------------------------------------------------------------------|
| `From<Ts...>`  | `Thunk<Ts...>`                                                             |
| `Empty`        | `Thunk<>`                                                                  |
| `Singleton<T>` | `Thunk<T>`                                                                 |
| `Pair<A, B>`   | `Thunk<A, B>`                                                              |
| `Iota<N>`      | `Thunk<integral_constant<size_t, 0>, ..., integral_constant<size_t, N-1>>` |
| `Repeat<T, N>` | `Thunk<T, T, ..., T>` (N times)                                            |

### Combinators

List manipulation:

| Combinator                         | Effect                                                                        |
|------------------------------------|-------------------------------------------------------------------------------|
| `Append<Us...>` / `Prepend<Us...>` | add types at the back / front                                                 |
| `Concat<Other>`                    | append another pipeline's results                                             |
| `Take<N>` / `Drop<N>`              | keep / discard the first N (saturating)                                       |
| `At<N>` / `Front` / `Back`         | select one element (as a `Singleton`)                                         |
| `First` / `Second`                 | pair accessors (aliases of `At<0>` / `At<1>`)                                 |
| `Reverse`                          | reverse the order                                                             |
| `Unique`                           | drop duplicates, keep first occurrences                                       |
| `SortBy<Cmp, Proj>`                | sort via reflection; presets `SortBySize`, `SortBySizeDec`, `SortByAlignment` |
| `Chunk<N>`                         | group into sub-Thunks of up to N                                              |
| `SplitBy<Pred>`                    | split into sub-Thunks at matching elements (delimiters are dropped)           |
| `Zip<Other>`                       | pair up index-wise, shorter side wins                                         |
| `Enumerate`                        | zip with indices                                                              |
| `Unpack`                           | unpack template instantiations (`std::tuple<A, B>` → `A, B`)                  |
| `Filter<Pred>`                     | keep matching elements                                                        |
| `Find<Pred>`                       | first match as `Singleton`, or `Empty`                                        |
| `Map<Pipeline>`                    | transform each element through a pipeline                                     |
| `FlatMap<Pipeline>`                | map to lists, then flatten — monadic bind                                     |
| `Flatten`                          | merge one level of nested Thunks — monadic join                               |
| `Bind<Other>`                      | Cartesian product as `Pair`s                                                  |
| `Match<Cases...>`                  | pattern-match a single type (see showcase)                                    |
| `Force`                            | evaluate pipeline here and now                                                |

### Predicates

Predicates are ordinary combinators producing a boolean value, so they compose
with the same `|`:

|                          |                                                                         |
|--------------------------|-------------------------------------------------------------------------|
| `Is<Trait>`              | lift a standard or user trait (`::value` or `::Value`) into the algebra |
| `Fn<MetaFn>`             | lift a metafunction (`::type` or `::Type`) — for `Map`                  |
| `Not`                    | invert a verdict — **postfix**: `Is<P> \| Not`                          |
| `And<L, R>` / `Or<L, R>` | combine two predicates over the same element                            |

A composed predicate is a first-class dictionary entry:

```cpp
static constexpr auto IsCleanPointer =
    typus::Fn<std::remove_cvref> | typus::Is<std::is_pointer>;

// ... | typus::Filter<IsCleanPointer> | ...
// ... | typus::Any<IsCleanPointer>
```

### Terminators

Terminators end a pipeline and produce a plain value or a type:

| Terminator                      | Result                                                                 |
|---------------------------------|------------------------------------------------------------------------|
| `Get<pipeline>`                 | the single contained type                                              |
| `Materialize<pipeline>`         | a `TypeTuple`                                                          |
| `Size`                          | element count (`size_t`)                                               |
| `Contains<T>` / `IndexOf<T>`    | membership (`bool`) / position (`size_t`) (not-found -> index == size) |
| `All<P>` / `Any<P>` / `None<P>` | quantifiers (empty list: `true` / `false` / `true`)                    |
| `Sum` / `Product`               | fold numeric type-level values (empty: `size_t{0}` / `size_t{1}`)      |

## Showcase

### Match: type-level pattern matching

`Match` maps one type through an ordered list of `Case`s to a replacement,
with an optional `Default`. It operates on a single element, but you can 
vectorize it with `Map`:

```cpp
struct SmallStorage {};
struct HeapStorage  {};
struct PtrStorage   {};

template <typename T>
struct Small : std::bool_constant<(sizeof(T) <= 4)> {};

using Storage = typus::Materialize
    < typus::From<std::uint8_t, double, char*, float>
    | typus::Map<typus::Match
        < typus::Case<typus::Is<Small>, SmallStorage>
        , typus::Case<typus::Is<std::is_pointer>, PtrStorage>
        , typus::Default<HeapStorage>
        >
      >
    >;
// TypeTuple over Thunk<SmallStorage, HeapStorage, PtrStorage, SmallStorage>
```

Cases are checked in order; the first hit wins. No match and no `Default` is
a compile error with a message naming the unmatched type.

### FlatMap: the list monad at type level

`Map` is one-to-one. `FlatMap` is one-to-many: the pipeline you pass must
produce a `Thunk` per element, and the results are spliced together
(`FlatMap<P> = Map<P> | Flatten`):

```cpp
// One metafunction, many results per input:
template <typename T>
struct WithCvVariants {
  using type = typus::base::Thunk<T, const T, volatile T>;
};

using Expanded = typus::Materialize
    < typus::From<int, char>
    | typus::FlatMap
      < typus::Fn<WithCvVariants>
      >
    >;
// Thunk<int, const int, volatile int, char, const char, volatile char>
```

### TypeTuple::As: a door into your application

`As<Target>` rebinds the final pack into any variadic template — `std::variant`,
`std::tuple`, or your own:

```cpp
// Collect every small trivially-copyable type into a variant:
using Message = typus::Materialize
<
  AllProtocolTypes   // a dictionary pipeline defined elsewhere
  | typus::Filter<typus::Is<std::is_trivially_copyable>>
  | typus::SortBySize
>::As<std::variant>;
```

One expression: gather, filter, layout-sort, rebind. The pipeline is a named
`constexpr` value at every intermediate step, so each stage is independently
testable with a `static_assert`.

## Extending typus

Meet the following requirements and your type is indistinguishable from a
built-in — it composes with `|`, enters `Filter` / `Map` / `Case` slots, and
participates in chains evaluation.

### Writing your own combinator

1. Inherit `typus::tag::Combinator`.
2. Provide `template <model::Thunk T> using Apply = ...` mapping
   `Thunk<...>` to `Thunk<...>`.
3. That's it. The base tag makes ADL find `operator|`.

```cpp
namespace myapp {

namespace detail {

// A combinator that keeps every other element:
struct EveryOther final : typus::tag::Combinator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<typus::base::Thunk<Ts...>> {
    static consteval std::meta::info Picked() {
      std::vector<std::meta::info> kept;
      auto types = typus::detail::ToArray<Ts...>();
      auto size = types.size();
      
      for (std::size_t i = 0; i < size; i += 2) {
        kept.push_back(types[i]);
      }
      
      return typus::detail::FromRange(std::move(kept));
    }

    using Type = [:Picked():];
  };

 public:
  template <typus::model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

inline constexpr auto EveryOther = myapp::EveryOther{};

}  // namespace myapp

// Composes like a native:
using R = typus::Materialize<typus::From<int, float, double> | myapp::EveryOther>;
// Thunk<int, double>
```

Element-level combinators (like `Is` / `Fn` / `Not`) specialize `Impl` on
`base::Singleton<T>` instead and are meant to run inside `Map` / `Filter` —
follow that pattern if your operation is per-element.

C++26 reflection is fully available inside `Apply`: `std::meta::members_of`,
`identifier_of`, `substitute`. Name-driven combinators (map `KeyPress` to 
`KeyPressHandler` found by identifier lookup in a namespace) are entirely 
buildable in user code.

### Writing your own terminator

Same shape, different tag and output: inherit `typus::tag::ValueTerminator`,
and let `Apply` produce a type exposing a static value (`::value` or
`::Value` — both spellings are accepted):

```cpp
namespace myapp {

namespace detail {

// Total size in bytes of all types in the pipeline:
struct TotalBytes final : typus::tag::ValueTerminator {
 private:
  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<typus::base::Thunk<Ts...>> {
    static constexpr std::size_t value = (0uz + ... + sizeof(Ts));
  };

 public:
  template <typus::model::Thunk T>
  using Apply = Impl<T>;
};

}  // namespace detail

inline constexpr auto TotalBytes = myapp::TotalBytes{};

}  // namespace myapp

static_assert((typus::From<std::uint32_t, std::uint64_t> | myapp::TotalBytes) == 12);
```

When anchored data meets your terminator on `|`, the eager overload fires,
forces the pipeline, and reads the value — exactly as it does for `Size`.

## Development

This project builds with a pinned toolchain provided entirely through Nix — you
do not install compilers or CMake yourself.

### Prerequisites

- [Nix](https://nixos.org/download) with flakes enabled
- [direnv](https://direnv.net) (recommended, for automatic shell activation)

### Environment

With direnv, the dev shell loads automatically:

```bash
direnv allow
```

This enters the `gcc` dev shell. The default shell is a tooling shell; builds use the pinned per-compiler shells.

Without direnv, enter a shell explicitly:

```bash
nix develop .#gcc     # GCC 16
```

### Build & test

```bash
just warmup        # configure dev, generate compile_commands.json for clangd
just build         # configure + build (default preset: dev)
just test          # build + run unit tests
```

Build or test a specific preset:

```bash
just build release
just test dev-asan
```

Run a subset of tests by name:

```bash
just test-filter <regex>
```

Sanitizer presets are available: `dev-asan` (ASan + UBSan) and `dev-tsan` (TSan).

### Formatting

```bash
just fmt           # format all sources (treefmt)
```

### Custom build options

Project-local compile options go in `cmake/UserOptions.cmake`. This file is
yours — it is created once and never overwritten by template updates. For flags
that only apply to this project's own code:

```cmake
target_compile_options(project_options INTERFACE -fyour-flag)
```

For flags that consumers of the library must also use:

```cmake
target_compile_options(typus PUBLIC -fyour-flag)
```

## Using this library

### With Nix

Add this flake as an input and use its package output:

```nix
inputs.typus.url = "github:SeveraTheDuck/typus";
# packages.${system}.default carries the installed library + CMake config
```

### With CMake (any toolchain, no Nix required)

Build and install into a prefix:

```bash
cmake -B build -D typus_ENABLE_INSTALL=ON
cmake --build build
cmake --install build --prefix <prefix>
```

Then consume it from another project:

```cmake
find_package(typus CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE severa::typus)
```

**Note:** typus requires a compiler with P2996 reflection support
(`-freflection` on GCC 16+). The flag propagates automatically through the
CMake target — consumers only need the compiler.

## Updating from template

```bash
copier update --trust -a .copier/answers.cpp.yaml
```

<!-- TODO: document centralised template rollout via the dedicated repository -->

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

[MIT](LICENSES/MIT.txt) © Alexander Antipov
