/**
 * @file greeting.hpp
 * @brief Greeting utilities for typus.
 */

#pragma once

#include <string>
#include <string_view>

namespace typus {

/**
 * @brief Builds a greeting message for the given name.
 *
 * @param name Name to greet.
 * @return A greeting of the form "Hello, <name>!".
 */
[[nodiscard]] std::string Greeting(std::string_view name);

}  // namespace typus
