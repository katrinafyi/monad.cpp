#pragma once

// we want a type-level map to resolve "keys",
// which are specified in template arguments to "values".
// keys may be a type and values may be an index

#include <array>
#include <cassert>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "tuple.hpp"

template <typename K, typename V, std::pair<K, V>... entries> struct map_t {

  template <K k, V v> map_t<K, V, entries..., std::pair<K, V>(k, v)> add() {
    return map_t<K, V, entries..., std::pair<K, V>(k, v)>{};
  }

  template <
      K k, size_t map_t_num_results = std::tuple_size_v<decltype(std::tuple_cat(
               (conditional_v<k == std::get<0>(entries)>(
                   std::tuple<V>{std::get<1>(entries)}, std::tuple<>()))...))>>
    requires(1 == map_t_num_results)
  V get() {
    return std::get<0>(std::tuple_cat((conditional_v<k == std::get<0>(entries)>(
        std::tuple<V>{std::get<1>(entries)}, std::tuple<>()))...));
  }
};

template <std::size_t N = 64> struct map_key {
  using buffer = std::array<char, N>;
  const buffer b;

  constexpr static buffer copy_sv(std::string_view s) noexcept {
    if (!(s.length() < N)) {
      (std::abort(), (void)"compile-time error: map_key: string too long!");
    }
    buffer b;
    b.fill(0); // zero-ing full array is required for comparison
    std::copy(s.begin(), s.end(), b.begin());
    return b;
  }

  template <typename T>
    requires std::constructible_from<std::string_view, T>
  consteval map_key(T &&t) noexcept : b(copy_sv(std::string_view{t})){};

  constexpr auto operator<=>(const map_key &) const = default;
  constexpr operator std::string() const & { return std::string{b.data()}; };

  friend std::ostream &operator<<(std::ostream &stream, const map_key &k) {
    return stream << std::string_view{k.b.cbegin(), k.b.cend()};
  }
};
