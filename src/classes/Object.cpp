#include "Object.hpp"
#include "Utility.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

const Shader *Object::getShader() const { return m_shader.get(); }

void Object::setShader(Shader *shader) {
  m_shader = std::shared_ptr<Shader>{shader};
}
