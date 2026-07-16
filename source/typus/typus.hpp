/**
 * @file typus.hpp
 * @author SeveraTheDuck
 * @brief Umbrella header: includes the entire Typus library.
 *
 * Typus is a C++26 header-only type-list metaprogramming library. Pipelines are
 * built lazily with `operator|` over a `base::Thunk`, composed of combinators,
 * and terminated by extraction into a value or a frozen `base::TypeTuple`.
 *
 * Include this to pull in everything; individual headers may be included
 * directly for finer-grained dependencies.
 */
#pragma once

// Core types and the pipe operator: Thunk, TypeTuple, aliases, operator|.
#include "base/empty.hpp"
#include "base/pair.hpp"
#include "base/pipe.hpp"
#include "base/singleton.hpp"
#include "base/thunk.hpp"
#include "base/type_tuple.hpp"

// Combinators: lazy Thunk -> Thunk transformations composed mid-pipeline.
#include "combine/and.hpp"
#include "combine/append.hpp"
#include "combine/at.hpp"
#include "combine/back.hpp"
#include "combine/bind.hpp"
#include "combine/chunk.hpp"
#include "combine/concat.hpp"
#include "combine/drop.hpp"
#include "combine/enumerate.hpp"
#include "combine/filter.hpp"
#include "combine/find.hpp"
#include "combine/flat_map.hpp"
#include "combine/flatten.hpp"
#include "combine/fn.hpp"
#include "combine/force.hpp"
#include "combine/front.hpp"
#include "combine/is.hpp"
#include "combine/map.hpp"
#include "combine/match.hpp"
#include "combine/not.hpp"
#include "combine/or.hpp"
#include "combine/pair.hpp"
#include "combine/prepend.hpp"
#include "combine/reverse.hpp"
#include "combine/sort.hpp"
#include "combine/split_by.hpp"
#include "combine/tag.hpp"
#include "combine/take.hpp"
#include "combine/unique.hpp"
#include "combine/unpack.hpp"
#include "combine/zip.hpp"

// Makers: entry points that lift raw types into a pipeline.
#include "make/empty.hpp"
#include "make/from.hpp"
#include "make/iota.hpp"
#include "make/pair.hpp"
#include "make/repeat.hpp"
#include "make/singleton.hpp"

// Concepts: the role vocabulary (Thunk, Anchored, Combinator, Terminator, ...).
#include "model/anchored.hpp"
#include "model/combinator.hpp"
#include "model/operation.hpp"
#include "model/pipe_expr.hpp"
#include "model/terminator.hpp"
#include "model/thunk.hpp"

// Terminators: extract a value, or freeze the pipeline into a TypeTuple.
#include "terminate/all.hpp"
#include "terminate/any.hpp"
#include "terminate/contains.hpp"
#include "terminate/get.hpp"
#include "terminate/index_of.hpp"
#include "terminate/materialize.hpp"
#include "terminate/none.hpp"
#include "terminate/product.hpp"
#include "terminate/size.hpp"
#include "terminate/sum.hpp"
#include "terminate/tag.hpp"
