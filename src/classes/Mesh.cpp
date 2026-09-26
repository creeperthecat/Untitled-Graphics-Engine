#include "Mesh.hpp"
#include "Shader.hpp"
#include "Utility.hpp"
#include <iostream>
#include <string>

void Mesh::draw(const Shader *shader, bool skipTextures,
                unsigned int drawMode) const {
  bindTextures(shader, skipTextures);

  glBindVertexArray(m_buffer.getVAO());
  glDrawElements(drawMode, m_indices.size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}

void Mesh::bindTextures(const Shader *shader, bool skipTextures) const {
  unsigned int diffuseNr{1};
  unsigned int specularNr{1};

  if (!skipTextures) {
    for (unsigned int i{0}; i < m_textures.size(); ++i) {
      glActiveTexture(GL_TEXTURE0 + i);

      std::string number{};
      std::string name{m_textures[i].type};

      if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (name == "texture_specular") {
        number = std::to_string(specularNr++);
      }

      if (!shader->validFetch(name + number)) {
        std::cerr << "Invalid shader skipped: " << name << number << "\n";
        continue;
      }

      shader->setInt((name + number), i);
      glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
  }
}
