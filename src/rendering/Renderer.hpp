#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <map>
#include <string>
#include <vector>

#include <nanovg.h>

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../settings.hpp"
#include "../world/World.hpp"
#include "ShaderManager.hpp"
#include "UI.hpp"
#include "stb_image.h"

namespace rendering {

class Renderer {

public:
  explicit Renderer(engine::Engine& engine);

  virtual bool init();
  virtual void cleanup();

  void prepareFrame();
  void sendFrame();

protected:
  engine::Engine& engine;

  glm::vec3 clearColor;

  GLuint compileShader(GLenum shaderType, std::string filename);

  template <class T>
  void glBufferDataVector(GLenum target, const std::vector<T>& v, GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), &v[0], usage);
  }
};
}

#endif
