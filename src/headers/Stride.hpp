#pragma once

#include <cstddef>
#include <initializer_list>
#include <vector>

/// Stride class which defines attributes of a shape.
/// Used in Shape for defining vertex attributes.
class Stride {
public:
  /// Constructor for Stride.
  /// @param attributes Vector of integers which define the size of attributes
  /// for a single vertex. For example, xyz + rgb would be {3,3}.
  /// @param pointers Vector of pointers, which define which attributes should
  /// be used when drawing the shape. Default behaviour is all attributes.
  Stride(const std::vector<int> &attributes,
         const std::vector<int> &pointers = {})
      : m_attributes{attributes},
        m_pointers{pointers.empty() ? generatePointers() : pointers} {}

  /// Initialiser list constructor for Stride. Only initialises attributes.
  /// @param attributes Vector of integers defining the size of attributes for a
  /// single vertex.
  Stride(std::initializer_list<int> attributes)
      : m_attributes{attributes}, m_pointers{generatePointers()} {}

  /// Specialised copy constructor for Stride which redefines which attributes
  /// to use. Useful for when you want to use the same vertices for a different
  /// shape, but exclude some if its attributes.
  Stride(const Stride &stride, const std::vector<int> &pointers)
      : Stride{stride.m_attributes, pointers} {}

  /// Returns total size in bytes of a vertex.
  /// Used when assigning attributes in a Buffer.
  std::size_t size() const;
  /// Gets the length of an attribute at index i
  int attributeLength(int i) const;
  /// Gets the size in bytes of an attribute at index i
  std::size_t attributeSize(int i) const;
  /// Gets total number of attributes.
  int count() const;
  /// Returns array of pointers for iteration over attributes which are in
  /// use.
  /// By default, iterates consecutively over all attributes.
  const std::vector<int> &getAttributes() const;

private:
  // Internal members
  std::vector<int> m_attributes{};
  std::vector<int> m_pointers{};

  // Helper function for initialisation of attribute pointers
  std::vector<int> generatePointers() const;
};
