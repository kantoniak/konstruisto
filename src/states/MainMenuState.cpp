#include "MainMenuState.hpp"

namespace states {

MainMenuState::MainMenuState(engine::Engine& engine, rendering::Renderer& renderer, MapState& mapState)
    : GameState(engine), renderer(renderer), mapState(mapState) {
}

void MainMenuState::init() {
  glClearColor(0.086f, 0.086f, 0.086f, 1.f);
}

void MainMenuState::update(std::chrono::milliseconds delta) {
  delta = delta;
}

void MainMenuState::render() {
  renderer.prepareFrame();

  engine.getDebugInfo().onRenderWorldEnd();
  const glm::vec2 viewportSize = engine.getWindowHandler().getViewportSize();
  nvgBeginFrame(engine.getUI().getContext(), viewportSize.x, viewportSize.y, 1.f);

  nvgTextAlign(engine.getUI().getContext(), NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
  nvgFontFace(engine.getUI().getContext(), rendering::UI::FONT_SSP_BOLD);
  nvgFontSize(engine.getUI().getContext(), 100.0f);

  nvgFillColor(engine.getUI().getContext(), engine.getUI().getPrimaryTextColor());
  nvgText(engine.getUI().getContext(), 100, 150, PROJECT_NAME, nullptr);

  nvgFontSize(engine.getUI().getContext(), 42.0f);
  for (int i = 0; i < MainMenuState::buttonsCount; i++) {
    renderButton(buttons[i], glm::vec2(100, 250 + i * 80), 20);
  }

  nvgEndFrame(engine.getUI().getContext());
  glFlush();

  engine.getDebugInfo().onRenderUIEnd();
  renderer.sendFrame();
}

void MainMenuState::onKey(int key, int scancode, int action, int mods) {
  key = scancode = action = mods = 0;
}

void MainMenuState::onMouseButton(int button, int action, int mods) {
  mods = 0;
  if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE) {
    return;
  }
  int hitButton = hitTestButton(engine.getWindowHandler().getMousePosition());
  switch (hitButton) {
  default:
    return;
  case 0:
    startGame();
    return;
  case 1:
    engine.stop();
    return;
  }
}

void MainMenuState::onScroll(double xoffset, double yoffset) {
  xoffset = yoffset = 0;
}

void MainMenuState::onWindowFocusChange(int focused) {
  focused = 0;
}

void MainMenuState::onWindowResize(int width, int height) {
  width = height = 0;
}

void MainMenuState::renderButton(const char* label, glm::vec2 pos, unsigned short padding) {
  constexpr unsigned short buttonHeight = 60;
  float labelWidth = nvgTextBounds(engine.getUI().getContext(), 0, 0, label, nullptr, nullptr);

  nvgFillColor(engine.getUI().getContext(), engine.getUI().getAccentColor());
  nvgBeginPath(engine.getUI().getContext());
  nvgRect(engine.getUI().getContext(), pos.x, pos.y, labelWidth + 2 * padding, buttonHeight);
  nvgFill(engine.getUI().getContext());

  nvgFillColor(engine.getUI().getContext(), engine.getUI().getPrimaryTextColor());
  nvgBeginPath(engine.getUI().getContext());
  nvgText(engine.getUI().getContext(), pos.x + padding, pos.y + buttonHeight / 2, label, nullptr);
  nvgFill(engine.getUI().getContext());
}

int MainMenuState::hitTestButton(glm::vec2 mouse) {
  nvgFontSize(engine.getUI().getContext(), 42.0f);

  glm::vec4 button; // x1 (top-left), y1, x2, y2
  for (int i = 0; i < MainMenuState::buttonsCount; i++) {
    float labelWidth = nvgTextBounds(engine.getUI().getContext(), 0, 0, buttons[i], nullptr, nullptr);
    button = glm::vec4(100, 250 + i * 80, 100 + labelWidth + 2 * 20, 250 + i * 80 + 60);
    if (mouse.x < button.x || mouse.y < button.y || mouse.x > button.z || mouse.y > button.w) {
      continue;
    }
    return i;
  }

  return -1;
}

void MainMenuState::startGame() {
  engine.pushState(mapState);
}
}