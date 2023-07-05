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

  template <typename F, typename B = inner_type<
                            typename std::invoke_result_t<F, const A &>>>
    requires is_monad<T> && std::regular_invocable<F, const A &>
  monad_ops<T, B> then(F &&f) const & {
    return monad_ops<T, B>{monad<T>::template bind<A, B>(f, m)};
  }

  operator T<A>() && { return std::move(m); }
};

