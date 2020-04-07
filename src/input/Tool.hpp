#ifndef INPUT_TOOL_HPP
#define INPUT_TOOL_HPP

#include <glm/glm.hpp>

#include "../rendering/SceneRenderer.hpp"

namespace input {

class Tool {

public:
  virtual ~Tool()= default;;
  virtual void update() noexcept {};
  virtual void render([[maybe_unused]] rendering::SceneRenderer& renderer) noexcept {};
};
}

#endif
