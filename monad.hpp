#pragma once

#include <functional>
#include <type_traits>

template <template <typename> typename T> struct functor;

template <template <typename> typename T> struct applicative;

template <template <typename> typename T> struct monad;

namespace {
class A {};
class B {};
class C {};
} // namespace

template <template <typename> typename T>
concept is_functor = requires(const T<A> &obj, std::function<B(const A &)> f) {
  { functor<T>::template fmap<A, B>(f, obj) } -> std::same_as<T<B>>;
};

template <template <typename> typename T>
concept is_applicative = requires(const T<std::function<B(const A &)>> mf,
                                  const T<A> &ma, const A &a) {
  { applicative<T>::template pure<A>(a) } -> std::same_as<T<A>>;
  { applicative<T>::template ap<A, B>(mf, ma) } -> std::same_as<T<B>>;
};

template <template <typename> typename T>
concept is_monad =
    requires(std::function<T<B>(const A &)> f, const T<A> &fa, const A &a) {
      { monad<T>::template pure<A>(a) } -> std::same_as<T<A>>;
      { monad<T>::template bind<A, B>(f, fa) } -> std::same_as<T<B>>;
    };

template <template <typename> typename M>
  requires is_monad<M>
struct applicative<M> {
  template <typename A> static M<A> pure(const A &a) {
    return monad<M>::pure(a);
  }

  template <typename A, typename B>
  static M<B> ap(const M<std::function<B(const A &)>> mf, const M<A> &ma) {
    return monad<M>::template bind<std::function<B(const A &)>, B>(
        [&](std::function<B(const A &)> f) -> M<B> {
          return monad<M>::template bind<A, B>(
              [&](const A &a) -> M<B> { return monad<M>::pure(f(a)); }, ma);
        },
        mf);
  }
};

template <template <typename> typename M>
  requires is_applicative<M>
struct functor<M> {
  template <typename A, typename B>
  static M<B> fmap(std::function<B(const A &)> f, const M<A> &m) {
    return applicative<M>::template ap<A, B>(
        applicative<M>::template pure<std::function<B(const A &)>>(f), m);
  }
};

namespace {

template <typename...> struct inner_type_impl;

template <template <typename...> typename C, typename T, typename... Args>
struct inner_type_impl<C<T, Args...>> {
  using type = T;
  // template <typename... Args2> using tmpl = C<Args2...>;
};

} // namespace

template <typename T>
using inner_type = typename inner_type_impl<std::remove_cvref_t<T>>::type;
// template <typename T> using outer_type = typename inner_type_impl<std::remove_cvref_t<T>>::tmpl;
