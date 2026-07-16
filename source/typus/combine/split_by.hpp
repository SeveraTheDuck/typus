/**
 * @file split_by.hpp
 * @author SeveraTheDuck
 * @brief Combinator that splits the pipeline into chunks separated by a predicate.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/combinator.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/holds.hpp>
#include <typus/details/to_array.hpp>

#include <meta>
#include <utility>
#include <vector>

namespace typus {

namespace detail {

template <model::Combinator auto Predicate>
struct SplitBy final : tag::Combinator {
 private:
  template <typename... Ts>
  [[nodiscard]] static consteval std::meta::info SplitInfo() {
    std::vector<std::meta::info> chunks;
    std::vector<std::meta::info> current_chunk;

    template for (constexpr auto t : ToArray<Ts...>()) {
      if constexpr (Holds<Predicate, typename[:t:]>) {
        chunks.push_back(FromRange(std::move(current_chunk)));
        current_chunk.clear();
      } else {
        current_chunk.push_back(t);
      }
    }

    chunks.push_back(FromRange(std::move(current_chunk)));

    return FromRange(std::move(chunks));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:SplitInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Splits the pipeline into multiple Thunks using a predicate as a delimiter.
 *
 * The element that matches the predicate is discarded, acting as the boundary
 * between the new chunks. Consecutive or edge delimiters produce empty chunks.
 *
 * @tparam Predicate The condition identifying the split boundaries.
 *
 * @par Example
 * @code
 * constexpr auto p = typus::From<int, float, void, double> |
 *                    typus::SplitBy<typus::Is<std::is_void>>;
 * // Result: Thunk<Thunk<int, float>, Thunk<double>>
 * @endcode
 */
template <model::Combinator auto Predicate>
inline constexpr auto SplitBy = detail::SplitBy<Predicate>{};

}  // namespace typus
