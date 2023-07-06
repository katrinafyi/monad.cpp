#include <string>
#include <variant>

#include "tuple.hpp"

int main() {
  std::tuple<int, bool, std::string> asd{100, true, "hehehe"};
  auto a = go<1>(asd, false);
  print(asd);
  print(a);
}
