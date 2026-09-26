#pragma once

#include "SceneRenderer.hpp"
#include <array>
#include <memory>
#include <string_view>
#include <vector>

/// Namespace which stores global variables.
/// Used to store user variables as well as the renderer.
namespace Globals {
/// Unique pointer for scene renderer to be used in user scripts
inline std::unique_ptr<SceneRenderer> renderer{nullptr};
/// Must be initialised before creating any Renderer or Object.
inline std::string programPath{};

// User variables

constexpr std::string_view windowName{"Instancing Demo"};
constexpr std::string_view asteriodModel{"rock/rock.obj"};
constexpr std::string_view planetModel{"planet/planet.obj"};

constexpr std::string_view asteroidVertex{"asteroid.vert"};
constexpr std::string_view asteroidFragment{"asteroid.frag"};

constexpr std::string_view planetVertex{"planet.vert"};
constexpr std::string_view planetFragment{"planet.frag"};

inline unsigned int asteroidID{};
inline unsigned int planetID{};

constexpr int instanceCount{10000};
inline std::vector<glm::mat4> modelMatrices(instanceCount);
} // namespace Globals
