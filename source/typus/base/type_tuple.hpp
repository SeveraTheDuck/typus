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
 * @brief Primary template for TypeTuple.
 *
 * TypeTuple acts as a structured interface to extract and inspect types
 * from an evaluated Thunk.
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
   * @brief Rebinds the types into a target variadic template.
   *
   * @tparam Target The template class to apply the types to (e.g., std::tuple, std::variant).
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
