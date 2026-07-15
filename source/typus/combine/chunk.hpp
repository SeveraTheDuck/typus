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
    chunks.reserve((size + N - 1) / N);

    for (std::size_t i = 0; i < size; i += N) {
      std::size_t current_chunk_size = std::min(N, size - i);
      std::span<std::meta::info> span{types.data() + i, current_chunk_size};
      chunks.push_back(FromRange(span));
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

template <std::size_t N>
inline constexpr auto Chunk = detail::Chunk<N>{};

}  // namespace typus
