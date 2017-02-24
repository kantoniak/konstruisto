#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../engine/Engine.hpp"
#include "Camera.hpp"
#include "ShaderManager.hpp"
#include "WindowHandler.hpp"

namespace rendering {

class Renderer {

public:
  Renderer(engine::Engine& engine, Camera& camera);

  bool init();
  void cleanup();

  void renderWorld();

protected:
  engine::Engine& engine;
  Camera& camera;

  glm::vec3 clearColor;

  GLuint shaderProgram;
  GLuint transformLoc;
  GLuint VBO, VAO;
  GLuint texture;
};
}

#endif
