/**
 * @file tag.hpp
 * @author SeveraTheDuck
 * @brief Definition of base tags for Typus combinators.
 */
#pragma once

namespace typus::tag {

/**
 * @brief Base class tag used to identify pipeline combinators.
 *
 * Any structure deriving from `Combinator` satisfies `model::Combinator`:
 * it can stand on the right of `operator|`, compose point-free with other
 * combinators, and fill predicate/pipeline slots (Filter, Map, Case).
 * Inherit this tag to plug user-defined combinators into Typus pipelines.
 */
struct Combinator {};

}  // namespace typus::tag
