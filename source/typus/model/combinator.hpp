#pragma once

#include <typus/combine/tag.hpp>

#include <concepts>

namespace typus::model {

template <typename T>
concept Combinator = std::derived_from<T, tag::Combinator>;

}  // namespace typus::model
