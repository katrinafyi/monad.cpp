#include <string>
#include <variant>

#include "tuple.hpp"

int main() {
  std::tuple<int, bool, std::string> asd{100, true, "hehehe"};
  print_tuple(asd);
  auto a = tuple_replace<1>(asd, false);
  print_tuple(a);
  a = tuple_replace<2>(a, "fsaji");
  print_tuple(a);
  a = tuple_map<2>(
      a, [](const std::string &x) -> std::string { return x + " yoted asdf!"; });
  print_tuple(a);
}
