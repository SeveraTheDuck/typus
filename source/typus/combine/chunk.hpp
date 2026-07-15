/**
 * @file chunk.hpp
 * @author SeveraTheDuck
 * @brief Combinator that groups types into sub-pipelines of a fixed size.
 */
#pragma once

#include "tag.hpp"

#include <typus/base/thunk.hpp>
#include <typus/model/thunk.hpp>

#include <typus/details/from_range.hpp>
#include <typus/details/to_array.hpp>

#include <algorithm>
#include <cstddef>
#include <meta>
#include <span>
#include <vector>

namespace typus {

namespace detail {

template <std::size_t N>
struct Chunk final : tag::Combinator {
 private:
  static_assert(N > 0, "typus::Chunk requires a chunk size strictly greater than 0");

  template <typename... Ts>
  [[nodiscard]] static consteval std::meta::info ChunkedInfo() {
    auto types = ToArray<Ts...>();
    auto size = types.size();

    std::vector<std::meta::info> chunks;
    if (size == 0) {
      return FromRange(chunks);
    }

    chunks.reserve((size + N - 1) / N);

    for (std::size_t i = 0; i < size; i += N) {
      std::size_t current_chunk_size = std::min(N, size - i);
      auto chunk_span = std::span{types}.subspan(i, current_chunk_size);
      chunks.push_back(FromRange(chunk_span));
    }

    return FromRange(std::move(chunks));
  }

  template <typename>
  struct Impl;

  template <typename... Ts>
  struct Impl<base::Thunk<Ts...>> {
    using Type = [:ChunkedInfo<Ts...>():];
  };

 public:
  template <model::Thunk T>
  using Apply = Impl<T>::Type;
};

}  // namespace detail

/**
 * @brief Splits the pipeline into multiple `base::Thunk` chunks.
 *
 * Each resulting sub-thunk will contain up to N elements. The final chunk
 * may contain fewer than N elements if the total size is not perfectly divisible.
 *
 * @tparam N The maximum size of each chunk.
 *
 * @par Example
 * @code
 * // Thunk<Thunk<int, float>, Thunk<double>>
 * constexpr auto p = typus::From<int, float, double> | typus::Chunk<2>;
 * @endcode
 */
template <std::size_t N>
inline constexpr auto Chunk = detail::Chunk<N>{};

}  // namespace typus
