#include "TestState.hpp"

namespace states {

TestState::TestState(engine::Engine& engine) : GameState(engine), renderer(engine, camera, selection) {
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

  camera.init(initialPerspective, initialCamera);

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
  // TODO(kantoniak): Move mouse picking out of state
  // Selection
  glm::vec3 cameraPos = camera.getPosition();
  glm::vec3 ray = camera.getRay(engine.getWindowHandler().getMousePosition());
  glm::vec3 onPlane = cameraPos - (cameraPos.y / ray.y) * ray;
  if (onPlane.x < 0) {
    onPlane.x--;
  }
  if (onPlane.z < 0) {
    onPlane.z--;
  }
  if (!selection.isSelecting()) {
    selection.from(glm::ivec2(onPlane.x, onPlane.z));
    selection.to(glm::ivec2(onPlane.x, onPlane.z));
  } else {
    selection.to(glm::ivec2(onPlane.x, onPlane.z));
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