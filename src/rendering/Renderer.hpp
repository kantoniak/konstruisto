#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <map>
#include <optional>
#include <string>
#include <vector>

#include <nanovg.h>

#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../opengl/Shader.hpp"
#include "../settings.hpp"
#include "../world/World.hpp"
#include "ShaderManager.hpp"
#include "UI.hpp"
#include "stb_image.h"

namespace rendering {

using namespace opengl;

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

  [[nodiscard]] std::optional<Shader> compileShader(Shader::ShaderType shader_type, std::string filename) const;

  template <class T>
  void glBufferDataVector(GLenum target, const std::vector<T>& v, GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), &v[0], usage);
  }
};
}

#endif
