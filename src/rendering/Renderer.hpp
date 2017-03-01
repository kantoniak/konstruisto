#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../world/Camera.hpp"
#include "ShaderManager.hpp"

namespace rendering {

class Renderer {

public:
  Renderer(engine::Engine& engine, world::Camera& camera, input::Selection& selection);

  bool init();
  void cleanup();

  void renderWorld();

protected:
  engine::Engine& engine;
  world::Camera& camera;
  input::Selection& selection;

  glm::vec3 clearColor;

  // Terrain
  GLuint shaderProgram;
  GLuint transformLoc, selectionLoc;
  GLuint VBO, VAO;
  GLuint texture;

  // Buildings
  GLuint buildingsVAO, buildingsVBO;
  GLuint buildingsInstanceVBO;
  GLuint buildingsShaderProgram;
  GLuint buildingsTransformLoc;
  const unsigned int buildingsCount = 100;
  const unsigned int sideSize = 20;
  const unsigned int maxHeight = 6;
};
}

#endif
