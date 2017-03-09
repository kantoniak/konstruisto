#ifndef RENDERING_UI_HPP
#define RENDERING_UI_HPP

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <nanovg.h>

#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../world/World.hpp"
#include "ShaderManager.hpp"

namespace rendering {

class UI {

public:
  UI(engine::Engine& engine);

  bool init();
  void cleanup();

  NVGcontext* getContext();

  const NVGcolor getBackgroundColor() const;
  const NVGcolor getPrimaryTextColor() const;

  static constexpr const char* FONT_SSP_REGULAR = "Source Sans Pro Regular";
  static constexpr const char* FONT_SSP_BOLD = "Source Sans Pro Bold";

  static constexpr const unsigned char ICON_SIDE = 16;

protected:
  engine::Engine& engine;
  NVGcontext* nvgContext;

  NVGcolor bgColor;
  NVGcolor primaryTextColor;

  static constexpr const char* FONT_SSP_REGULAR_PATH = "assets/fonts/SourceSansPro/SourceSansPro-Regular.ttf";
  static constexpr const char* FONT_SSP_BOLD_PATH = "assets/fonts/SourceSansPro/SourceSansPro-Bold.ttf";
};
}

#endif
