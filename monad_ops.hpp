#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <variant>
#include <vector>

#include "monad.hpp"

template <template <typename> typename T, typename A> class monad_ops {
  T<A> m;

public:
  monad_ops(const T<A> &m) : m(m){};
  monad_ops(T<A> &&m) : m(std::move(m)){};

  // template <typename TA> monad_ops(TA &&m) : m(std::forward<TA>(m)) {}

  operator T<A>() && { return std::move(m); }

  template <typename F,
            typename B = inner_type<std::invoke_result_t<F, const A &>>>
    requires is_monad<T> && std::regular_invocable<F, const A &>
  monad_ops<T, B> then(F &&f) const & {
    return monad_ops<T, B>{monad<T>::template bind<A, B>(f, m)};
  }

  template <typename TB>
    requires std::same_as<A, std::monostate>
  monad_ops<T, inner_type<TB>> with(TB &&b) && {
    return monad_ops<T, inner_type<TB>>{std::forward<TB>(b)};
  }
};

template <template <typename> typename T>
  requires is_applicative<T>
monad_ops<T, std::monostate> monad_do() {
  return monad_ops<T, std::monostate>(applicative<T>::pure(std::monostate{}));
}

template <typename TA>
auto monad_do(TA &&a) {
  return monad_ops{std::forward<TA>(a)};
}
