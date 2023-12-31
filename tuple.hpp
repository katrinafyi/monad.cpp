#pragma once

#include <bits/utility.h>
#include <concepts>
#include <iostream>
#include <tuple>

template <bool cond, typename T, typename F>
  requires cond
inline T conditional_v(T &&t, F &&) {
  return t;
}

template <bool cond, typename T, typename F>
  requires(!cond)
inline F conditional_v(T &&, F &&f) {
  return f;
}

template <std::size_t N, typename... Ts>
using nth_t = typename std::tuple_element<N, std::tuple<Ts...>>::type;

template <std::size_t i, typename T, typename... Ts, std::size_t... is,
          typename Tuple2 = std::tuple<std::conditional_t<i == is, T, Ts>...>>
Tuple2 tuple_replace_impl(const std::tuple<Ts...> &tuple, T &&item,
                          std::index_sequence<is...>) {
  return std::make_tuple(
      (conditional_v<i == is>(std::forward<T>(item), std::get<is>(tuple)))...);
}

template <std::size_t i, typename... Ts,
          typename Is = std::index_sequence_for<Ts...>>
auto tuple_replace(const std::tuple<Ts...> &tuple, nth_t<i, Ts...> &&item) {
  return tuple_replace_impl<i, nth_t<i, Ts...>, Ts...>(
      tuple, std::forward<nth_t<i, Ts...>>(item), Is{});
}

template <std::size_t i, typename T, typename... Ts,
          typename Is = std::index_sequence_for<Ts...>>
auto tuple_replace(const std::tuple<Ts...> &tuple, T &&item) {
  return tuple_replace_impl<i, T, Ts...>(tuple, std::forward<T>(item), Is{});
}

/*
template <std::size_t i, typename T, typename... Ts, std::size_t... is,
          typename Tuple2 = std::tuple<std::conditional_t<i == is, T, Ts>...>>
Tuple2 tuple_emplace_impl(const std::tuple<Ts...> &tuple, T &&item,
                          std::index_sequence<is...>) {
  return std::make_tuple(
      (conditional_v<i == is>(std::forward<T>(item), std::get<is>(tuple)))...);
}

template <std::size_t i, typename... Args, typename... Ts,
          typename Is = std::index_sequence_for<Ts...>>
  requires std::constructible_from<nth_t<i, Ts...>, Args...>
auto tuple_emplace(const std::tuple<Ts...> &tuple, Args &&...item) {
  return tuple_emplace_impl<i, nth_t<i, Ts...>, Ts...>(
      tuple, {std::forward<Args>(item)...}, Is{});
}
*/

// template <std::size_t i, typename T, typename F, typename... Ts,
//           std::size_t... is, typename Tuple = std::tuple<Ts...>,
//           typename Tuple2 = std::tuple<std::conditional_t<i == is, T,
//           Ts>...>>
// Tuple2 tuple_map_impl(const Tuple &tuple, F &&f,
//                       std::index_sequence<is...>...) {
//   return std::make_tuple(
//       (conditional_v<i == is>(f(std::get<i>(tuple)),
//       std::get<is>(tuple)))...);
// }

template <std::size_t i, typename F, typename... Ts,
          typename Tuple = std::tuple<Ts...>,
          typename Is = std::index_sequence_for<Ts...>,
          typename T =
              std::invoke_result_t<F, const std::tuple_element_t<i, Tuple> &>>
auto tuple_map(const std::tuple<Ts...> &tuple, F &&f) {
  // return tuple_map_impl<i, T, F, Ts...>(tuple, std::forward<F>(f), Is{});
  return tuple_replace<i, T, Ts...>(tuple, f(std::get<i>(tuple)));
}

template <class Tuple, std::size_t... I>
void print_tuple(const Tuple &_tup, std::index_sequence<I...>) {
  std::cout << "(";
  (..., (std::cout << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
  std::cout << ")\n";
}

template <class... T> void print_tuple(const std::tuple<T...> &_tup) {
  print_tuple(_tup, std::make_index_sequence<sizeof...(T)>());
}
