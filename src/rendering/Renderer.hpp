#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../engine/Engine.hpp"
#include "ShaderManager.hpp"
#include "WindowHandler.hpp"

namespace rendering {

class Renderer {

public:
  Renderer(engine::Engine& engine);

  bool init();
  void cleanup();

  void renderWorld();

protected:
  engine::Engine& engine;

  glm::vec3 clearColor;

  GLuint shaderProgram;
  GLuint VBO, VAO;
};
}

#endif
