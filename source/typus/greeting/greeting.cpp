#include <typus/greeting/greeting.hpp>

#include <string>

namespace typus {

std::string Greeting(std::string_view name) {
  return "Hello, " + std::string{name} + "!";
}

}  // namespace typus
