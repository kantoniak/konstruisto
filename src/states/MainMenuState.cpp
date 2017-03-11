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
  engine.getUI().startFrame();
  NVGcontext* context = engine.getUI().getContext();

  nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
  nvgFontFace(context, rendering::UI::FONT_SSP_BOLD);
  nvgFontSize(context, 100.0f);

  nvgFillColor(context, engine.getUI().getPrimaryTextColor());
  nvgText(context, 100, 150, PROJECT_NAME, nullptr);

  nvgFontSize(context, 42.0f);
  for (int i = 0; i < MainMenuState::buttonsCount; i++) {
    renderButton(buttons[i], glm::vec2(100, 250 + i * 80), 20);
  }

  engine.getUI().endFrame();
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
  NVGcontext* context = engine.getUI().getContext();
  constexpr unsigned short buttonHeight = 60;
  float labelWidth = nvgTextBounds(context, 0, 0, label, nullptr, nullptr);

  nvgFillColor(context, engine.getUI().getAccentColor());
  nvgBeginPath(context);
  nvgRect(context, pos.x, pos.y, labelWidth + 2 * padding, buttonHeight);
  nvgFill(context);

  nvgFillColor(context, engine.getUI().getPrimaryTextColor());
  nvgBeginPath(context);
  nvgText(context, pos.x + padding, pos.y + buttonHeight / 2, label, nullptr);
  nvgFill(context);
}

int MainMenuState::hitTestButton(glm::vec2 mouse) {
  NVGcontext* context = engine.getUI().getContext();
  nvgFontSize(context, 42.0f);

  glm::vec4 button; // x1 (top-left), y1, x2, y2
  for (int i = 0; i < MainMenuState::buttonsCount; i++) {
    float labelWidth = nvgTextBounds(context, 0, 0, buttons[i], nullptr, nullptr);
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