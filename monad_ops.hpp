#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <variant>
#include <vector>

#include "monad.hpp"

template <typename... input_t>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<input_t>()...));

template <template <typename> typename T, typename A> class monad_do {
  T<A> m;

public:
  monad_do(const T<A> &m) : m(m){};
  monad_do(T<A> &&m) : m(std::move(m)){};

  // template <typename TA> monad_ops(TA &&m) : m(std::forward<TA>(m)) {}

  operator T<A>() && { return std::move(m); }

  template <typename F,
            typename B = inner_type<std::invoke_result_t<F, const A &>>>
    requires is_monad<T> && std::regular_invocable<F, const A &>
  monad_do<T, B> then(F &&f) const & {
    return monad_do<T, B>{monad<T>::template bind<A, B>(f, m)};
  }

  template <typename TB>
    requires std::same_as<A, std::monostate> &&
             std::constructible_from<monad_do<T, inner_type<TB>>, TB>
  monad_do<T, inner_type<TB>> with(TB &&b) const & {
    return monad_do<T, inner_type<TB>>{std::forward<TB>(b)};
  }

  template <typename TB, typename B = inner_type<TB>>
    requires is_monad<T> && is_complete_v<std::tuple_size<A>>
  monad_do<T, tuple_cat_t<A, std::tuple<B>>> with(TB &&mb) && {
    using AB = tuple_cat_t<A, std::tuple<B>>;
    using M = monad<T>;

    return monad_do<T, AB>{M::template bind<A, AB>(
        [&](const A &a) -> T<AB> {
          return M::template bind<B, AB>(
              [&](const B &b) -> T<AB> {
                return M::template pure<AB>(
                    std::tuple_cat(a, std::make_tuple(b)));
              },
              mb);
        },
        m)};
  }

  template <typename TB, typename B = inner_type<TB>>
    requires is_monad<T> && std::same_as<T<B>, TB> &&
             (!is_complete_v<std::tuple_size<A>>) &&
             (!std::same_as<A, std::monostate>)
  monad_do<T, std::tuple<A, B>> with(TB &&mb) && {
    using AB = std::tuple<A, B>;
    using M = monad<T>;

    return monad_do<T, AB>{M::template bind<A, AB>(
        [&](const A &a) -> T<AB> {
          return M::template bind<B, AB>(
              [&](const B &b) -> T<AB> {
                return M::template pure<AB>(std::make_tuple(a, b));
              },
              mb);
        },
        m)};
  }
};

template <template <typename> typename T>
  requires is_applicative<T>
monad_do<T, std::monostate> mdo() {
  return monad_do<T, std::monostate>(applicative<T>::pure(std::monostate{}));
}

template <template <typename> typename T, typename A>
monad_do<T, A> mdo(T<A> &&a) {
  return monad_do<T, A>{std::forward<T<A>>(a)};
}

template <typename TA, typename = inner_type<TA>> auto mdo(TA &&a) {
  return monad_do{std::forward<TA>(a)};
}
