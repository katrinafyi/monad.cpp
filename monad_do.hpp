#pragma once

#include <variant>

namespace monad_do_impl {
  template <typename T>
  int f(int) { return 0; }

  template <typename T>
  struct B {};

  template <typename T>
  struct DoResult {};

  template <typename T>
  struct DoBind {};
}

