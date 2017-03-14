#include "MapPauseState.hpp"

namespace states {

MapPauseState::MapPauseState(engine::Engine& engine, world::World& world, rendering::Renderer& renderer)
    : GameState(engine), world(world), renderer(renderer) {
}

void MapPauseState::init() {
}

void MapPauseState::update(std::chrono::milliseconds delta) {
  delta = delta;
}

void MapPauseState::render() {
  renderer.prepareFrame();

  renderer.renderWorld();
  engine.getDebugInfo().onRenderWorldEnd();

  NVGcontext* context = engine.getUI().getContext();
  engine.getUI().startFrame();
  const glm::vec2 viewportSize = engine.getWindowHandler().getViewportSize();

  nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  nvgFontFace(context, rendering::UI::FONT_SSP_BOLD);
  nvgFontSize(context, 70.0f);

  const data::City& city = world.getMap().getCurrentCity();
  float cityNameWidth = nvgTextBounds(context, 0, 0, city.name.c_str(), nullptr, nullptr);

  nvgFillColor(context, engine.getUI().getBackgroundColor());
  nvgBeginPath(context);
  nvgRect(context, viewportSize.x / 2 - cityNameWidth / 2 - 40, viewportSize.y / 2 - 40, cityNameWidth + 80, 80);
  nvgFill(context);

  nvgFillColor(context, engine.getUI().getPrimaryTextColor());
  nvgBeginPath(context);
  nvgText(context, viewportSize.x / 2, viewportSize.y / 2, city.name.c_str(), nullptr);
  nvgFill(context);

  engine.getUI().endFrame();
  renderer.sendFrame();
}

void MapPauseState::onKey(int key, int scancode, int action, int mods) {
  scancode = mods = 0;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    engine.popState();
  }
}

void MapPauseState::onMouseButton(int button, int action, int mods) {
  button = action = mods = 0;
}

void MapPauseState::onScroll(double xoffset, double yoffset) {
  xoffset = yoffset = 0;
}

void MapPauseState::onWindowFocusChange(int focused) {
  focused = 0;
}

void MapPauseState::onWindowResize(int width, int height) {
  world.getCamera().updateAspect(width / (float)height);
}
}