#include <string_view>
#include <variant>

#include "map.hpp"

int main() {
  map_t<map_key<>, map_key<>> b{};
  auto x = b.add<"ads", "asdj9j2190">();
  auto y = x.add<"101", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaasd hi there">();
  auto aa = y.get<"101">();
  // auto aaaaa = y.get<"101a">();
  std::cout << aa << std::endl;
}
