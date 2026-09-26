#include "Shape.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

Shape::Shape(const std::vector<float> &shape, const Stride &stride)
    : m_shape{shape}, m_stride{stride} {
  assert(!m_shape.empty() && "Empty shape provided in constructor.");
  assert(size() % m_stride.size() == 0 && "Misaligned attributes in shape.");
}

Shape::Shape(std::vector<float> &&shape, const Stride &stride)
    : m_shape{std::move(shape)}, m_stride{stride} {
  assert(!m_shape.empty() && "Empty shape provided in constructor.");
  assert(size() % m_stride.size() == 0 && "Misaligned attributes in shape.");
}

Shape::Shape(const Shape &shape, const Stride &stride)
    : Shape{shape.m_shape, stride} {
  assert(stride.size() == shape.m_stride.size() &&
         "Misaligned stride (different size from original shape).");
}

int Shape::vertexCount() const {
  return static_cast<int>(size() / m_stride.size());
}

const float *Shape::getVertices() const { return m_shape.data(); }

std::size_t Shape::size() const { return m_shape.size() * sizeof(float); }

const Stride &Shape::getStride() const { return m_stride; }

std::vector<float> Shape::generateShape(std::vector<Vertex> &&shape) const {
  // All vertices must be same size
  std::size_t element_size{shape.front().size()};
  assert(std::ranges::all_of(shape,
                             [element_size](const Vertex &v) {
                               return v.size() == element_size;
                             }) &&
         "Misaligned attributes in vertex array (attribute sizes different).");

  // Create and populate new vector to move into shape
  std::vector<float> vec(shape.front().size() * shape.size());

  auto vertex{vec.begin()};
  auto addVertex{[&vertex](const Vertex &v) {
    std::ranges::move(v, vertex);
    std::advance(vertex, v.size());
  }};

  std::ranges::for_each(shape, addVertex);
  return vec;
}

std::vector<float>
Shape::generateShape(const std::vector<Vertex> &shape) const {
  // All vertices must be same size
  std::size_t element_size{shape.front().size()};
  assert(std::ranges::all_of(shape,
                             [element_size](const Vertex &v) {
                               return v.size() == element_size;
                             }) &&
         "Misaligned attributes in vertex array (attribute sizes different).");

  // Create and populate new vector to move into shape
  std::vector<float> vec(shape.front().size() * shape.size());

  auto vertex{vec.begin()};
  auto addVertex{[&vertex](const Vertex &v) {
    std::ranges::copy(v, vertex);
    std::advance(vertex, v.size());
  }};

  std::ranges::for_each(shape, addVertex);
  return vec;
}
