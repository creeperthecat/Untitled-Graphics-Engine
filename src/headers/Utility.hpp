#pragma once

#include <cstddef>
#include <glad/gl.h>
#include <type_traits>

/// Converts a numerical value to size_t.
template <typename T> constexpr std::size_t toUZ(T value) {
  static_assert(std::is_integral<T>() || std::is_enum<T>());
  return static_cast<std::size_t>(value);
}

/// Converts a numerical value to an integer.
template <typename T> constexpr GLint toInt(T value) {
  static_assert(std::is_integral<T>() || std::is_enum<T>());
  return static_cast<GLint>(value);
}
