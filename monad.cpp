#include <functional>
#include <iostream>
#include <optional>

#include "monad.hpp"
#include "monad_do.hpp"
#include "monad_ops.hpp"

template <> struct monad<std::optional> {
  template <typename A> static std::optional<A> pure(const A &a) { return {a}; }

  template <typename A, typename B>
  static std::optional<B> bind(std::function<std::optional<B>(const A &)> f,
                               const std::optional<A> &a) {
    return !a ? std::optional<B>{} : f(*a);
  }
};

template <typename T> using vector = std::vector<T>;

template <> struct monad<vector> {
  template <typename A> static vector<A> pure(const A &a) { return {a}; }

  template <typename A, typename B>
  static vector<B> bind(std::function<vector<B>(const A &)> f,
                        const vector<A> &xs) {
    vector<B> ys{};
    for (const A &x : xs) {
      const vector<B> temp = f(x);
      ys.insert(ys.end(), temp.begin(), temp.end());
    }
    return ys;
  }
};

int main(int argc, char **argv) {
  static_assert(is_monad<std::optional>);
  static_assert(is_applicative<std::optional>);
  static_assert(is_functor<std::optional>);

  static_assert(is_monad<vector>);
  static_assert(is_applicative<vector>);
  static_assert(is_functor<vector>);

  std::optional<int> a{10};
  auto b = functor<std::optional>::template fmap<int, int>(
      [](auto a) -> int { return a + 1000; }, a);
  std::cout << a.value() << std::endl;
  std::cout << b.value() << std::endl;

  std::vector<int> as{
      1, 2, 3, 4, 5,
  };
  std::vector<int> bs = monad<vector>::bind<int, int>(
      [](auto a) -> vector<int> {
        return {a * 10, a * 100};
      },
      as);
  for (auto x : as)
    std::cout << x << ' ';
  std::cout << std::endl;
  for (auto x : bs)
    std::cout << x << ' ';
  std::cout << std::endl;

  std::vector<int> cs = functor<vector>::fmap<int, int>(std::identity(), as);
  for (auto x : cs)
    std::cout << x << ' ';

  vector<bool> asd = monad_ops<vector, int>({1, 2, 3, 4})
                         .then(monad<vector>::pure<int>)
                         .then([](auto) -> vector<bool> { return {true}; });
  auto x = monad_ops<vector, int>({1, 2});

  int na;
  // auto aasdsa = monad_ops<std::reference_wrapper, int>(std::ref(na)).then(1);

  return 0;
}
