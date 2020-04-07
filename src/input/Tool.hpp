#ifndef INPUT_TOOL_HPP
#define INPUT_TOOL_HPP

#include <glm/glm.hpp>

#include "../rendering/SceneRenderer.hpp"

namespace input {

class Tool {

public:
  virtual ~Tool() = default;

  virtual void update() noexcept = 0;
  virtual void render([[maybe_unused]] rendering::SceneRenderer& renderer) noexcept = 0;

  virtual void on_mouse_button(int button, int action, int mods) noexcept = 0;
};
}

#endif
