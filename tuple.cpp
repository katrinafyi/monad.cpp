#include <filesystem>
#include <memory>
#include <string>
#include <variant>

#include "tuple.hpp"

int main() {
  std::tuple<int, bool, std::string> asd{100, true, "hehehe"};
  std::cout << typeid(asd).name() << std::endl;
  print_tuple(asd);

  std::tuple<int, bool, std::string> a = tuple_replace<1>(asd, false);
  std::cout << typeid(a).name() << std::endl;
  print_tuple(a);

  a = tuple_replace<2>(a, {"fsaji"});
  std::cout << typeid(a).name() << std::endl;
  print_tuple(a);

  std::tuple<std::unique_ptr<std::string>, bool, std::string> aa =
      tuple_replace<0>(a, std::make_unique<std::string>("yoted"));
  std::cout << typeid(aa).name() << std::endl;
  print_tuple(aa);

  std::tuple<int, bool, std::filesystem::path> a2 =
      tuple_map<2>(a, [](const std::string &x) -> std::filesystem::path {
        return (std::filesystem::path{x} / " yoted asdf!");
      });
  std::cout << typeid(a2).name() << std::endl;
  print_tuple(a2);
}
