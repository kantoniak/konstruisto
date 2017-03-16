#include "UI.hpp"

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

namespace rendering {
UI::UI(engine::Engine& engine) : engine(engine) {
}

bool UI::init() {

  nvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
  if (nvgContext == nullptr) {
    engine.getLogger().severe("Could not initialize NanoVG context.");
    return false;
  }
  if (-1 == nvgCreateFont(nvgContext, FONT_SSP_REGULAR, FONT_SSP_REGULAR_PATH)) {
    engine.getLogger().severe("Could not load font %s from %s.", FONT_SSP_REGULAR, FONT_SSP_REGULAR_PATH);
    return false;
  }
  if (-1 == nvgCreateFont(nvgContext, FONT_SSP_BOLD, FONT_SSP_BOLD_PATH)) {
    engine.getLogger().severe("Could not load font %s from %s.", FONT_SSP_BOLD, FONT_SSP_BOLD_PATH);
    return false;
  }

  logoImage = nvgCreateImage(nvgContext, "assets/textures/ui/logo.png", NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
  if (0 == logoImage) {
    engine.getLogger().severe("Could not load texture from %s.", "assets/textures/ui/logo.png");
    return false;
  }
  
  bgColor = nvgRGB(34, 34, 34);
  primaryTextColor = nvgRGB(255, 255, 255);
  accentColor = nvgRGB(200, 113, 55);

  engine.getLogger().info("UI initialized.");
  return true;
}
void UI::cleanup() {
  if (!nvgContext) {
    return;
  }
  if (logoImage) {
    nvgDeleteImage(nvgContext, logoImage);
  }
  nvgDeleteGL3(nvgContext);
}

NVGcontext* UI::getContext() {
  return nvgContext;
}

void UI::startFrame() {
  const glm::vec2 viewportSize = engine.getWindowHandler().getViewportSize();
  nvgBeginFrame(engine.getUI().getContext(), viewportSize.x, viewportSize.y, 1.f);
}

void UI::endFrame() {
  nvgEndFrame(engine.getUI().getContext());
  glFlush();
  engine.getDebugInfo().onRenderUIEnd();
}

const NVGcolor UI::getBackgroundColor() const {
  return bgColor;
}

const NVGcolor UI::getPrimaryTextColor() const {
  return primaryTextColor;
}

const NVGcolor UI::getAccentColor() const {
  return accentColor;
}

void UI::renderLogo(float x, float y) {
  NVGpaint paint = nvgImagePattern(nvgContext, x, y, 640, 120, 0, logoImage, 1.0f); // nvglImageHandleGL3 gives texture handle
  nvgFillPaint(nvgContext, paint);
  nvgBeginPath(nvgContext);
  nvgRect(nvgContext, x, y, 640, 120);
  nvgFill(nvgContext);
}
}