#include "TestState.hpp"

namespace states {

TestState::TestState(engine::Engine& engine) : GameState(engine), renderer(engine, world, selection) {
  this->suspended = false;
};

void TestState::init() {
  if (!engine.getWindowHandler().createMainWindow()) {
    engine.stop();
    return;
  }

  world::PerspectiveState initialPerspective;
  initialPerspective.fovy = 45.f;
  initialPerspective.aspect = engine.getWindowHandler().getViewportRatio();
  initialPerspective.zNear = 0.1f;
  initialPerspective.zFar = 10000.f;

  data::CameraState initialCamera;
  initialCamera.lookAt = glm::vec3(0);
  initialCamera.distance = 30;
  initialCamera.rotationAroundX = M_PI / 4.f;
  initialCamera.rotationAroundY = 0;

  world.getCamera().init(initialPerspective, initialCamera);
  world.init();

  geometry.init(world);

  if (!renderer.init()) {
    engine.stop();
    return;
  }
};

void TestState::cleanup() {
  renderer.cleanup();
}

void TestState::update(std::chrono::milliseconds delta) {
  delta = delta;

  glm::ivec2 selectionEnd;
  if (geometry.hitField(engine.getWindowHandler().getMousePosition(), selectionEnd)) {
    if (!selection.isSelecting()) {
      selection.from(selectionEnd);
    }
    selection.to(selectionEnd);
  }
};

void TestState::render() {
  renderer.renderWorld();
};

void TestState::onMouseButton(int button, int action, int mods) {
  mods = 0;
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    selection.start(selection.getFrom());
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    selection.stop();
  }
}
}