#include <project_name/hello/hello.hpp>

#include <cstddef>

int main() {
  static constexpr std::size_t kIterations = 1000000;
  for (std::size_t i = 0; i < kIterations; ++i) {
    hello::Print();
  }
}
