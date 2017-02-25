#include "TestState.hpp"

namespace states {

TestState::TestState(engine::Engine& engine) : GameState(engine), renderer(engine, camera) {
  this->suspended = false;
};

void TestState::init() {
  if (!engine.getWindowHandler().createMainWindow()) {
    engine.stop();
    return;
  }

  rendering::PerspectiveState initialPerspective;
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
};

void TestState::render() {
  renderer.renderWorld();
};
}