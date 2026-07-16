/**
 * @file type_tuple.hpp
 * @author SeveraTheDuck
 * @brief A materialized interface for interacting with evaluated Thunks.
 */
#pragma once

#include "thunk.hpp"

#include <typus/model/thunk.hpp>

#include <cstddef>

namespace typus::base {

/**
 * @brief The frozen, immutable result of Materialize.
 *
 * A read-only interface to inspect an evaluated Thunk. Constructible only from
 * a Thunk (the primary template is undefined), so a TypeTuple cannot be
 * hand-built or converted back into a Thunk — freezing is one-way by design.
 *
 * @tparam Thunk A type satisfying the model::Thunk concept.
 */
template <model::Thunk /*Thunk*/>
struct TypeTuple;

/**
 * @brief Specialization of TypeTuple for base::Thunk.
 *
 * @tparam Ts The types held within the Thunk.
 */
template <typename... Ts>
struct TypeTuple<Thunk<Ts...>> final {
  /**
   * @brief Rebinds the contained types into a target variadic template.
   *
   * The one-way door out of typus into any external template (std::tuple,
   * std::variant, rusty::Enum, ...). Does not produce a new TypeTuple.
   *
   * @tparam Target The template to apply the types to.
   */
  template <template <typename...> typename Target>
  using As = Target<Ts...>;

  /**
   * @brief Accesses the type at a specific index using C++26 pack indexing.
   *
   * @tparam N The index of the type to retrieve.
   */
  template <std::size_t N>
  using At = Ts...[N];

  /**
   * @brief The number of types contained in the tuple.
   */
  static constexpr auto Size = sizeof...(Ts);
};

}  // namespace typus::base
