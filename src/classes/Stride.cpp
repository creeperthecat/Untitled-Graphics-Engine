#include "Stride.hpp"
#include "Utility.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>

std::size_t Stride::size() const {
  return toUZ(std::accumulate(m_attributes.begin(), m_attributes.end(), 0)) *
         sizeof(float);
}

int Stride::attributeLength(int i) const {
  assert(0 <= i && i < std::ssize(m_attributes) &&
         "Out-of-bounds access to attribute.");
  assert((m_pointers.empty() || std::find(m_pointers.begin(), m_pointers.end(),
                                          i) != m_pointers.end()) &&
         "Out-of-bounds access to attribute.");
  return m_attributes.data()[i];
}

std::size_t Stride::attributeSize(int i) const {
  assert(0 <= i && i < std::ssize(m_attributes) &&
         "Out-of-bounds access to attribute.");
  assert((m_pointers.empty() || std::find(m_pointers.begin(), m_pointers.end(),
                                          i) != m_pointers.end()) &&
         "Out-of-bounds access to attribute.");
  return toUZ(attributeLength(i)) * sizeof(float);
}

int Stride::count() const { return static_cast<int>(m_attributes.size()); }

const std::vector<int> &Stride::getAttributes() const { return m_pointers; }

std::vector<int> Stride::generatePointers() const {
  std::vector<int> vec(m_attributes.size());
  std::iota(vec.begin(), vec.end(), 0);
  return vec;
}
