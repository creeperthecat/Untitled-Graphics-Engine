#pragma once
#include <glm/glm.hpp>
#include <string>

/// Namespace used to define model data.
namespace ModelData {
/// The Vertex struct defines a vertex in a model file, using its position,
/// normal and UV coordinates.
struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoords{};
};

/// The Texture struct stores properties of a texture needed for rendering.
struct Texture {
  unsigned int id{};
  std::string type{};
  std::string path{};
};
}; // namespace ModelData
