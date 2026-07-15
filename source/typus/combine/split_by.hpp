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

template <model::Combinator auto Predicate>
inline constexpr auto SplitBy = detail::SplitBy<Predicate>{};

}  // namespace typus
