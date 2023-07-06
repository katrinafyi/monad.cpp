#pragma once

#include <bits/utility.h>
#include <concepts>
#include <iostream>
#include <tuple>

template <std::size_t i, std::size_t is, typename Tuple,
          typename T_is = std::tuple_element_t<is, Tuple>,
          typename T_i = std::tuple_element_t<i, Tuple>>
  requires(i != is)
T_is g(const T_is &old, T_i &&) {
  return old;
}

template <std::size_t i, std::size_t is, typename Tuple,
          typename T_is = std::tuple_element_t<is, Tuple>,
          typename T_i = std::tuple_element_t<i, Tuple>>
  requires(i == is)
T_is g(const T_is &, T_i &&item) {
  return item;
}

template <std::size_t i, typename T, typename... Ts, std::size_t... is>
std::tuple<Ts...> go2_impl(const std::tuple<Ts...> &tuple, T &&item,
                           std::integer_sequence<std::size_t, is...>) {
  return std::make_tuple((g<i, is, std::tuple<Ts...>>(
      std::get<is>(tuple), std::forward<T>(item)))...);
}

template <std::size_t i, typename T, typename... Ts,
          typename Is = std::index_sequence_for<Ts...>>
  requires std::convertible_to<T, std::tuple_element_t<i, std::tuple<Ts...>>>
std::tuple<Ts...> go(const std::tuple<Ts...> &tuple, T &&item) {
  return go2_impl<i, T, Ts...>(tuple, std::forward<T>(item), Is{});
}

template <class TupType, std::size_t... I>
void print(const TupType &_tup, std::index_sequence<I...>) {
  std::cout << "(";
  (..., (std::cout << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
  std::cout << ")\n";
}

template <class... T> void print(const std::tuple<T...> &_tup) {
  print(_tup, std::make_index_sequence<sizeof...(T)>());
}
