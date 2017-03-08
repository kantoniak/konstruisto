#include "MapPauseState.hpp"

namespace states {

MapPauseState::MapPauseState(engine::Engine& engine, world::World& world, rendering::Renderer& renderer)
    : GameState(engine), world(world), renderer(renderer) {
  nvgContext = nullptr;
}

void MapPauseState::init() {
  nvgContext = engine.getUI().getContext();
}

void MapPauseState::update(std::chrono::milliseconds delta) {
  delta = delta;
}

void MapPauseState::render() {
  renderer.prepareFrame();

  renderer.renderWorld();
  engine.getDebugInfo().onRenderWorldEnd();

  if (!nvgContext) {
    engine.getLogger().debug("No NVG context");
    return;
  }
  const glm::vec2 viewportSize = engine.getWindowHandler().getViewportSize();
  nvgBeginFrame(nvgContext, viewportSize.x, viewportSize.y, 1.f);

  const NVGcolor bgColor = nvgRGB(34, 34, 34);
  const NVGcolor textColor = nvgRGB(255, 255, 255);

  nvgTextAlign(nvgContext, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  nvgFontFace(nvgContext, "Source Sans Pro Bold");
  nvgFontSize(nvgContext, 70.0f);

  const data::City& city = world.getMap().getCurrentCity();
  float cityNameWidth = nvgTextBounds(nvgContext, 0, 0, city.name.c_str(), nullptr, nullptr);

  nvgBeginPath(nvgContext);
  nvgRect(nvgContext, viewportSize.x / 2 - cityNameWidth / 2 - 40, viewportSize.y / 2 - 40, cityNameWidth + 80, 80);
  nvgFillColor(nvgContext, bgColor);
  nvgFill(nvgContext);

  nvgFillColor(nvgContext, textColor);
  nvgText(nvgContext, viewportSize.x / 2, viewportSize.y / 2, city.name.c_str(), nullptr);

  nvgEndFrame(nvgContext);
  glFlush();

  engine.getDebugInfo().onRenderUIEnd();
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
void MapPauseState::onWindowResize(int width, int height) {
  world.getCamera().updateAspect(width / (float)height);
}
}