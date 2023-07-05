#include <functional>
#include <iostream>
#include <optional>

template <template <typename> typename T> struct functor;

template <template <typename> typename T> struct applicative;

template <template <typename> typename T> struct monad;

struct A {};
struct B {};
struct C {};

template <template <typename> typename T>
concept is_functor = requires(const T<A> &obj, std::function<B(const A &)> f) {
  { functor<T>::fmap(f, obj) } -> std::same_as<T<B>>;
};

template <template <typename> typename T>
concept is_applicative = requires(std::function<C(const A &, const B &)> f,
                                  const T<A> &fa, const T<B> &fb, A a) {
  { applicative<T>::pure(a) } -> std::same_as<T<A>>;
  { applicative<T>::liftA2(f, fa, fb) } -> std::same_as<T<C>>;
};

template <template <typename> typename T>
concept is_monad =
    requires(std::function<T<B>(const A &)> f, const T<A> &fa, A a) {
      { monad<T>::pure(a) } -> std::same_as<T<A>>;
      { monad<T>::bind(f, fa) } -> std::same_as<T<B>>;
    };

template <> struct monad<std::optional> {
  template <typename A> static std::optional<A> pure(A a) { return {a}; }

  template <typename A, typename B>
  static std::optional<B> bind(std::function<std::optional<B>(const A &)> f,
                               const std::optional<A> &a) {
    return !a ? std::optional<B>{} : f(*a);
  }
};

template <template <typename> typename M>
  requires is_monad<M>
struct functor<M> {
  template <typename A, typename B>
  static M<B> fmap(std::function<B(const A &)> f, const M<A> &m) {
    return monad<M>::template bind<A, B>(
        [&](const A &a) -> M<B> { return monad<M>::pure(f(a)); }, m);
  }
};

template <template <typename> typename M>
  requires is_monad<M>
struct applicative<M> {
  template <typename A> static M<A> pure(A a) { return monad<M>::pure(a); }

  template <typename A, typename B, typename C>
  static M<C> liftA2(std::function<C(const A &, const B &)> f, const M<A> &fa,
                     const M<B> &fb) {
    return monad<M>::bind(
        [&](const A &a) -> M<C> {
          functor<M>::fmap([&](const B &b) -> C { return f(a, b); }, fb);
        },
        fa);
  }
};

// template <> struct functor<std::optional> {
//   template <typename A, typename B>
//   static std::optional<B> fmap(std::function<B(const A &)> f,
//                                const std::optional<A> &optional) {
//     std::optional<A> x{optional};
//
//     if (!x)
//       return x;
//     x = f(x.value());
//     return x;
//   }
// };

template <typename T> using vector = std::vector<T>;

template <> struct monad<vector> {
  template <typename A> static vector<A> pure(A a) { return {a}; }

  template <typename A, typename B>
  static vector<B> bind(std::function<vector<B>(const A &)> f,
                        const vector<A> &xs) {
    vector<B> ys{};
    for (const A& x : xs) {
      for (const B& y : f(x))
      {
        ys.push_back(y);
      }
    }
    return ys;
  }
};

int main(int argc, char **argv) {
  static_assert(is_monad<std::optional>);
  static_assert(is_functor<std::optional>);
  static_assert(is_applicative<std::optional>);

  static_assert(is_monad<vector>);

  std::optional<int> a{10};
  auto b = functor<std::optional>::fmap<int, int>(
      [](auto a) -> int { return a + 1000; }, a);
  std::cout << a.value() << std::endl;
  std::cout << b.value() << std::endl;

  std::vector<int> as{1,2,3,4,5,};
  std::vector<int> bs = monad<vector>::bind<int,int>([](auto a) -> vector<int> { return {a * 10, a * 100}; }, as);
  for (auto x : as) std::cout << x << ' ';
  std::cout << std::endl;
  for (auto x : bs) std::cout << x << ' ';
  return 0;
}
