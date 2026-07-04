#pragma once

#include <typus/terminate/tag.hpp>

#include <concepts>

namespace typus::model {

template <typename T>
concept Terminator = std::derived_from<T, tag::ValueTerminator>;

}
