#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <cmath>
#include <iostream>

#include "../data/CameraState.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../rendering/Camera.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/WindowHandler.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(engine::Engine& engine) : GameState(engine), renderer(engine, camera) {
    this->suspended = false;
  };
  virtual ~TestState(){};

  virtual void init() {
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

  virtual void cleanup() {
    renderer.cleanup();
  }

  virtual void update(std::chrono::milliseconds delta) {
    delta = delta;
  };

  virtual void render() {
    renderer.renderWorld();
  };

private:
  rendering::Renderer renderer;
  rendering::Camera camera;
};
}

#endif
