#include "TestState.hpp"

namespace states {

TestState::TestState(engine::Engine& engine) : GameState(engine), renderer(engine, world, selection) {
  suspended = false;
};

void TestState::init() {
  if (!engine.getWindowHandler().createMainWindow()) {
    engine.stop();
    return;
  }

  world.getMap().createRandom(glm::ivec2(16, 16));

  world::PerspectiveState initialPerspective;
  initialPerspective.fovy = 45.f;
  initialPerspective.aspect = engine.getWindowHandler().getViewportRatio();
  initialPerspective.zNear = 0.1f;
  initialPerspective.zFar = 10000.f;

  data::CameraState initialCamera;
  initialCamera.lookAt = glm::vec3(world.getMap().getSize().x, 0, world.getMap().getSize().y) * 0.5f;
  initialCamera.distance = 80;
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
  world.cleanup();
}

void TestState::update(std::chrono::milliseconds delta) {
  delta = delta;

  if (rmbPressed) {
    handleMapDragging(delta);
  }
  if (rotatingClockwise) {
    handleRotatingAroundY(delta, true);
  }
  if (rotatingCounterClockwise) {
    handleRotatingAroundY(delta, false);
  }
  if (rotatingUpwards) {
    handleRotatingAroundX(delta, true);
  }
  if (rotatingDownwards) {
    handleRotatingAroundX(delta, false);
  }

  glm::ivec2 selectionEnd;
  if (geometry.hitField(engine.getWindowHandler().getMousePositionNormalized(), selectionEnd)) {
    if (!selection.isSelecting()) {
      selection.from(selectionEnd);
    }
    selection.to(selectionEnd);
  }
};

void TestState::render() {
  renderer.renderWorld(renderNormals);
};

void TestState::onKey(int key, int scancode, int action, int mods) {
  // TODO(kantoniak): Refactor TestState::onKey by the end of march. If it's April you can start laughing now :)
  scancode = mods = 0;

  if (key == GLFW_KEY_MINUS && action != GLFW_RELEASE) {
    world.getCamera().zoom(-5);
  }

  if (key == GLFW_KEY_EQUAL && action != GLFW_RELEASE) {
    world.getCamera().zoom(5);
  }

  // Rotation around Y
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    rotatingClockwise = true;
  } else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
    rotatingClockwise = false;
  }

  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    rotatingCounterClockwise = true;
  } else if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
    rotatingCounterClockwise = false;
  }

  // Rotation around X
  if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
    rotatingUpwards = true;
  } else if (key == GLFW_KEY_HOME && action == GLFW_RELEASE) {
    rotatingUpwards = false;
  }

  if (key == GLFW_KEY_END && action == GLFW_PRESS) {
    rotatingDownwards = true;
  } else if (key == GLFW_KEY_END && action == GLFW_RELEASE) {
    rotatingDownwards = false;
  }

  if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    renderNormals = !renderNormals;
  }
}

void TestState::onMouseButton(int button, int action, int mods) {
  mods = 0;
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    selection.start(selection.getFrom());
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    selection.stop();
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      rmbPressed = true;
      dragStart = engine.getWindowHandler().getMousePosition();
    } else {
      rmbPressed = false;
    }
  }
}

void TestState::onScroll(double xoffset, double yoffset) {
  xoffset = 0;
  world.getCamera().zoom(yoffset * 5);
}

void TestState::handleMapDragging(std::chrono::milliseconds delta) {
  const glm::vec2 mousePosition = engine.getWindowHandler().getMousePosition();
  const glm::vec2 dragDelta = mousePosition - dragStart;

  float dragSpeed = 0.2f * delta.count() / 1000.f;
  world.getCamera().move(dragSpeed * glm::vec3(dragDelta.x, 0.f, dragDelta.y));
}

void TestState::handleRotatingAroundY(std::chrono::milliseconds delta, bool clockwise) {
  float rotationDelta = M_PI * delta.count() / 1000.f;
  float modifier = (clockwise ? 1.f : -1.f);
  world.getCamera().rotateAroundY(rotationDelta * modifier);
}

void TestState::handleRotatingAroundX(std::chrono::milliseconds delta, bool upwards) {
  float rotationDelta = M_PI * delta.count() / 1000.f;
  float modifier = (upwards ? 1.f : -1.f);
  world.getCamera().rotateAroundX(rotationDelta * modifier);
}
}